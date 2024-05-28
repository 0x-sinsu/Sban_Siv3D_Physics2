#include <Siv3D.hpp> // Siv3D v0.6.14
#include <fstream>

/// @brief 文字
struct P2Glyph
{
	/// @brief 物理演算の物体
	P2Body body;

	/// @brief 文字のポリゴン
	MultiPolygon polygons;

	/// @brief 文字のポリゴンの凸包
	Polygon convexHull;

	/// @brief 初期位置
	Vec2 initialPos{ 0, 0 };

	/// @brief 落ちる順番
	int32 order = 0;

	// 2D 物理演算のワールド
	P2World world;

	void draw(const ColorF& color) const
	{
		if (body)
		{
			const auto [s, c] = FastMath::SinCos(body.getAngle());
			polygons.drawTransformed(s, c, body.getPos(), color);
		}
	}
};

/// @brief 物理演算用に多角形の凸包を計算
/// @param polygons 多角形
/// @return 凸包
static Polygon CalculateConvexHull(const MultiPolygon& polygons)
{
	Array<Vec2> points;

	for (const auto& polygon : polygons)
	{
		for (const auto& point : polygon.outer())
		{
			points << point;
		}
	}

	return Geometry2D::ConvexHull(points).simplified();
}

// 設定を読み込む関数
std::unordered_multimap<std::string, std::string> LoadSettings(const std::string& settingsFilePath) {
	std::unordered_multimap<std::string, std::string> settings;

	std::ifstream file(settingsFilePath);
	std::string line;

	while (std::getline(file, line)) {
		auto delimiterPos = line.find('=');
		if (delimiterPos != std::string::npos) {
			std::string key = line.substr(0, delimiterPos);
			std::string value = line.substr(delimiterPos + 1);
			settings.insert({ key, value });
		}
	}

	return settings;
}

s3d::Array<std::string> LoadText(const std::string& filePath) {
	s3d::Array<std::string> lines;
	std::ifstream file(filePath);
	std::string line;

	while (std::getline(file, line)) {
		lines.push_back(line);
	}

	return lines;
}

// std::vector<std::string> を s3d::Array<s3d::String> に変換する関数
s3d::Array<s3d::String> ConvertToS3DArray(const std::vector<std::string>& stdVector) {
	s3d::Array<s3d::String> s3dArray;
	for (const auto& stdString : stdVector) {
		s3dArray.push_back(s3d::Unicode::FromUTF8(stdString));
	}
	return s3dArray;
}

// 設定ファイルのパス
const std::string settingsfilepath = "./settings.conf";
s3d::String s3dsettingsfilepath = s3d::Unicode::FromUTF8(settingsfilepath);

/// @brief 各文字を生成
/// @param bottomCenter 最下層の中心位置
/// @param font フォント
/// @param texts 歌詞
/// @param fixedtext 固定文字
/// @return P2Glyph の配列
static Array<P2Glyph> GenerateGlyphs(const Vec2& bottomCenter, const Font& font, const Array<String>& texts, const Array<String>& fixedtext)
{
	Array<P2Glyph> allGlyphs;

	// 下から何行目か
	int32 lineCount = static_cast<int32>(texts.size());

	// std::unordered_multimapの初期化
	std::unordered_multimap<std::string, std::string> settingsMap;

	// 設定を読み込む
	auto settings = LoadSettings(settingsfilepath);

	double kanjiSize = 1.0; // デフォルト値を1.0として初期化
	auto itkanjiSize = settings.find("kanjiSize");
	if (itkanjiSize != settings.end()) {
		try {
			kanjiSize = std::stod(itkanjiSize->second);
		}
		catch (const std::invalid_argument) {
			kanjiSize = 1.0;
		}
	}

	for (const auto& text : texts)
	{
		const Array<PolygonGlyph> polygonGlyphs = font.renderPolygons(text);
		const Array<bool> isKanji = text.map([](const char32_t ch) {
			return
				((0x4E00 <= ch) && (ch <= 0x9FFF)) ||    // 基本的な漢字
				((0x3400 <= ch) && (ch <= 0x4DBF)) ||    // 拡張A
				((0x20000 <= ch) && (ch <= 0x2A6DF)) ||  // 拡張B
				((0x2A700 <= ch) && (ch <= 0x2B73F)) ||  // 拡張C
				((0x2B740 <= ch) && (ch <= 0x2B81F)) ||  // 拡張D
				((0x2B820 <= ch) && (ch <= 0x2CEAF)) ||  // 拡張E
				((0x2CEB0 <= ch) && (ch <= 0x2EBEF)) ||  // 拡張F
				((0xF900 <= ch) && (ch <= 0xFAFF)) ||    // CJK互換漢字
				((0x2F800 <= ch) && (ch <= 0x2FA1F));     // CJK互換漢字補助
			});

		Vec2 basePos{ 0, bottomCenter.y };

		Array<P2Glyph> line;

		for (size_t i = 0; i < polygonGlyphs.size(); ++i)
		{
			const auto& polygonGlyph = polygonGlyphs[i];
			const double scale = (isKanji[i] ? kanjiSize : 1.0);

			P2Glyph glyph;
			glyph.polygons = polygonGlyph.polygons.scaled(scale);
			glyph.convexHull = CalculateConvexHull(polygonGlyph.polygons);
			glyph.initialPos = (basePos + polygonGlyph.getBase(scale));
			glyph.order = lineCount;

			basePos.x += (polygonGlyph.xAdvance * scale);
			line << glyph;
		}

		if (line.isEmpty())
		{
			continue;
		}

		// 現在の行の幅
		const double lineLength = basePos.x;
		const double halfLineLength = (lineLength * 0.5);

		// 行を中心揃え
		for (auto& elem : line)
		{
			elem.initialPos.x -= halfLineLength;
			elem.initialPos.x += bottomCenter.x;
		}

		allGlyphs.insert(allGlyphs.end(), line.begin(), line.end());

		--lineCount;
	}

	for (const auto& fixed : fixedtext)
	{
		const Array<PolygonGlyph> polygonGlyphs = font.renderPolygons(fixed);
		const Array<bool> isKanji = fixed.map([](const char32_t ch) {return
			((0x4E00 <= ch) && (ch <= 0x9FFF)) ||    // 基本的な漢字
			((0x3400 <= ch) && (ch <= 0x4DBF)) ||    // 拡張A
			((0x20000 <= ch) && (ch <= 0x2A6DF)) ||  // 拡張B
			((0x2A700 <= ch) && (ch <= 0x2B73F)) ||  // 拡張C
			((0x2B740 <= ch) && (ch <= 0x2B81F)) ||  // 拡張D
			((0x2B820 <= ch) && (ch <= 0x2CEAF)) ||  // 拡張E
			((0x2CEB0 <= ch) && (ch <= 0x2EBEF)) ||  // 拡張F
			((0xF900 <= ch) && (ch <= 0xFAFF)) ||    // CJK互換漢字
			((0x2F800 <= ch) && (ch <= 0x2FA1F));     // CJK互換漢字補助
			});

		Vec2 basePos{ 0, bottomCenter.y };

		Array<P2Glyph> line;

		for (size_t i = 0; i < polygonGlyphs.size(); ++i)
		{
			const auto& polygonGlyph = polygonGlyphs[i];
			const double scale = (isKanji[i] ? kanjiSize : 1.0);
			P2Glyph glyph;
			glyph.polygons = polygonGlyph.polygons.scaled(scale);
			glyph.convexHull = CalculateConvexHull(polygonGlyph.polygons);
			glyph.initialPos = (basePos + polygonGlyph.getBase(scale));
			glyph.order = lineCount;

			basePos.x += (polygonGlyph.xAdvance * scale);
			line << glyph;
		}

		if (line.isEmpty())
		{
			continue;
		}

		// 現在の行の幅
		const double lineLength = basePos.x;
		const double halfLineLength = (lineLength * 0.5);

		// 行を中心揃え
		for (auto& elem : line)
		{
			elem.initialPos.x -= halfLineLength;
			elem.initialPos.x += bottomCenter.x;
		}

		allGlyphs.insert(allGlyphs.end(), line.begin(), line.end());

		--lineCount;
	}

	return allGlyphs;
}

void Main()
{
	Window::SetTitle(U"Sban_Siv3D_Physics2");

	// 設定ファイルの存在チェック
	if (!FileSystem::Exists(s3dsettingsfilepath)) {
		System::MessageBoxOK(U"settings.conf が存在しません。", MessageBoxStyle::Error);
	return;
	}

	// 背景色
	Scene::SetBackground(ColorF{ 0.0 });

	// std::unordered_multimapの初期化
	std::unordered_multimap<std::string, std::string> settingsMap;

	// 設定を読み込む
	auto settings = LoadSettings(settingsfilepath);

	// 設定ファイルの内容を取得
	s3d::Array<std::string> texts;
	s3d::Array<std::string> fixedtext;
	std::string simulationspeed;
	std::string fontpath;
	std::string fontsize;

	// 各設定の読み込み
        auto get_setting = [&](const std::string &key, std::string &value,
                              const std::string &errorMessage) {
          auto it = settings.find(key);
          if (it != settings.end()) {
            // 設定値が空文字列の場合にエラーメッセージを表示
            if (it->second.empty()) {
              System::MessageBoxOK(s3d::Unicode::Widen(errorMessage),
                                   MessageBoxStyle::Error);
              return false;
            }
            value = it->second;
          } else {
            System::MessageBoxOK(s3d::Unicode::Widen(errorMessage),
                                 MessageBoxStyle::Error);
            return false;
          }
          return true;
        };

        auto get_numericsetting = [&](const std::string &key, std::string &value,
                                     const std::string &errorMessage) {
          auto it = settings.find(key);
          if (it != settings.end()) {
            try {
              double tempValue = std::stod(it->second);
              value = std::to_string(tempValue);
            } catch (const std::invalid_argument &) {
              System::MessageBoxOK(s3d::Unicode::Widen(errorMessage),
                                   MessageBoxStyle::Error);
              return false;
            } catch (const std::out_of_range &) {
              System::MessageBoxOK(s3d::Unicode::Widen(errorMessage),
                                   MessageBoxStyle::Error);
              return false;
            }
          } else {
            System::MessageBoxOK(s3d::Unicode::Widen(errorMessage),
                                 MessageBoxStyle::Error);
            return false;
          }
          return true;
        };


	auto get_arraysetting = [&](const std::string &key, s3d::Array<std::string> &array, const std::string &errorMessage) {
		auto it = settings.find(key);
		if (it != settings.end()) {
			array = LoadText(it->second);
		} else {
			System::MessageBoxOK(s3d::Unicode::Widen(errorMessage), MessageBoxStyle::Error);
			return false;
		}
		return true;
	};
	
	// 設定の読み込みと存在チェック
	if (!get_arraysetting("lyricspath", texts, "lyricspath が設定ファイルにありません。")) {
		return;
	}
	if (!get_arraysetting("fixedtextpath", fixedtext, "fixedtextpath が設定ファイルにありません。")) {
		return;
	}
	if (!get_setting("simulationspeed", simulationspeed, "simulationspeed が設定ファイルにありません。")) {
		return;
	}
	if (!get_setting("fontpath", fontpath, "fontpath が設定ファイルにありません。")) {
		return;
	}
	if (!get_setting("fontsize", fontsize, "fontsize が設定ファイルにありません。")) {
		return;
	}
	if (!get_numericsetting("simulationspeed", simulationspeed, "simulationspeed の値が無効です。")) {
		return;
	}

	// フルスクリーン設定を確認
	bool fullscreen = false;
	auto itfullscreen = settings.find("fullscreen");
	if (itfullscreen != settings.end()) {
		fullscreen = (itfullscreen->second == "true");
	}

	if (fullscreen) {
		// フルスクリーンモードに設定
		Window::SetFullscreen(true);
        } else {
			int window_width;
			int window_height;

			// 設定からwindow_widthを取得
			auto itwindow_width = settings.find("window_width");
			if (itwindow_width != settings.end()) {
				try {
					double temp_width = std::stod(itwindow_width->second);
					if (temp_width == static_cast<int>(temp_width)) {
						window_width = static_cast<int>(temp_width);
					} else {
						System::MessageBoxOK(U"window_width の値が無効です。整数を指定してください。", MessageBoxStyle::Error);
						return;
					}
				} catch (const std::exception) {
				}
			} else {
				System::MessageBoxOK(U"window_width が設定ファイルにありません。", MessageBoxStyle::Error);
				return;
			}

			// 設定からwindowHeightを取得
            auto itwindow_height = settings.find("window_height");
            if (itwindow_height != settings.end()) {
				try {
					double temp_height = std::stod(itwindow_height->second);
					if (temp_height == static_cast<int>(temp_height)) {
						window_height = static_cast<int>(temp_height);
					} else {
						System::MessageBoxOK(U"window_height の値が無効です。整数を指定してください。", MessageBoxStyle::Error);
						return;
					}
				} catch (const std::exception) {
				}
			} else {
				System::MessageBoxOK(U"window_height が設定ファイルにありません。", MessageBoxStyle::Error);
				return;
			}

            Window::Resize(window_width, window_height);
		}
	

	// s3d::Array<s3d::String> に変換
	s3d::Array<s3d::String> s3d_texts = ConvertToS3DArray(texts);
	s3d::Array<s3d::String> s3d_fixedtext = ConvertToS3DArray(fixedtext);

	// std::stringからs3d::Stringへ変換
	s3d::String s3d_fontpath = s3d::Unicode::FromUTF8(fontpath);

	int intFontSize;
	try {
		intFontSize = std::stoi(fontsize);
	}
	catch (const std::exception) {
	}

	// Fontオブジェクトを初期化
	const Font font(intFontSize, s3d_fontpath);

	Array<P2Body> body;

	// シミュレーションスピード
	double speed{};
    if (!simulationspeed.empty()) {
        try {
            speed = std::stod(simulationspeed);
        }
        catch (const std::exception) {
          speed = 1;
        }
    }

	// 2D 物理演算のシミュレーションステップ（秒）
	constexpr double StepTime = (1.0 / 200.0);

	// 2D 物理演算のシミュレーション蓄積時間（秒）
	double accumulatedTime = 0.0;

	// 基本重力
	constexpr double basegravity = 160.0;

	// 2D 物理演算のワールド
	P2World world{ basegravity };

	// 画面の幅と高さ
	const double screenWidth = Scene::Width();
	const double screenHeight = Scene::Height();

	// テキストの幅と高さを計算
	const double textWidth = font(s3d_fixedtext).region().w;
	const double textHeight = font(s3d_fixedtext).region().h;

	// テキストを画面の中央に配置するための座標を計算
	const Vec2 textPos((screenWidth - textWidth) / 2, (screenHeight - textHeight) / 2);

	// 出力されたP2Glyphの配列を処理して物理ワールドに追加
	Array<P2Glyph> glyphs = GenerateGlyphs(Vec2{ 0, -1100 }, font, s3d_texts, Array<String>{});
	Array<P2Glyph> glyph2 = GenerateGlyphs(Vec2{ 0, 0 }, font, s3d_fixedtext, Array<String>{});

	for (auto& glyph : glyph2)
	{
		// 物理ボディを物理ワールドに追加
		glyph.body = world.createPolygon(P2Static, glyph.initialPos, glyph.convexHull);
	}

	Camera2D camera{ Vec2{ 0, 0 }, 1.0, CameraControl::None_ };

	// 何番まで登場しているか
	int32 activeOrder = 0;

	// 各行の登場タイミングを決めるためのストップウォッチ
	Stopwatch stopwatch{ StartImmediately::Yes };

	while (System::Update())
	{
		{
			// 何番まで登場させてよいかを計算
			const int32 t = static_cast<int32>(((stopwatch.ms() * speed) - 800) / 500);

			if (activeOrder < t) // 順番が進んだ
			{
				++activeOrder;

				// 同じ順番の文字の物理演算の物体を作成する
				for (auto& glyph : glyphs)
				{
					if (glyph.order == activeOrder)
					{
						// 文字の物理演算の物体を作成
						glyph.body = world.createPolygon(P2Dynamic, glyph.initialPos, glyph.convexHull);

						// 文字によって重力の倍率を変える
						glyph.body.setGravityScale(Random(1.0, 1.25));

						// 後ろが詰まらないように下向きの初速を与える
						glyph.body.setVelocity(Vec2{ 0, 90 });
					}
				}
			}
		}

		camera.update();
		{
			const auto t = camera.createTransformer();

			// 各 P2Glyph の描画
			for (const auto& glyph : glyphs)
			{
				glyph.draw(ColorF{ 0.93 });
			}
			for (const auto& glyph : glyph2)
			{
				glyph.draw(ColorF{ 0.93 });
			}
		}

		for (accumulatedTime += (Scene::DeltaTime() * speed); StepTime <= accumulatedTime; accumulatedTime -= StepTime)
		{
			// 2D 物理演算のワールドを StepTime 秒進める
			world.update(StepTime);
		}
	}
}
