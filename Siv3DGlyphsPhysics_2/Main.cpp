#include <Siv3D.hpp> // Siv3D v0.6.14
#include <fstream>
#include "UnicodeRanges.hpp"

// 設定ファイルのパス
const std::string settings_file_path = "./settings.conf";
String s3d_settings_file_path = Unicode::FromUTF8(settings_file_path);

// 設定を読み込む関数
std::unordered_map<std::string, std::string>
load_settings(const std::string& local_settings_file_path)
{
	std::unordered_map<std::string, std::string> settings;
	std::ifstream file(local_settings_file_path);
	std::string line;

	while (std::getline(file, line))
	{
		auto delimiter_pos = line.find('=');
		if (delimiter_pos != std::string::npos)
		{
			std::string key = line.substr(0, delimiter_pos);
			std::string value = line.substr(delimiter_pos + 1);
			settings.insert({key, value});
		}
	}

	return settings;
}

Array<std::string> load_text(const std::string& file_path)
{
	Array<std::string> lines;
	std::ifstream file(file_path);
	std::string line;

	while (std::getline(file, line))
	{
		lines.push_back(line);
	}

	return lines;
}


// std::vector<std::string> を s3d::Array<s3d::String> に変換する関数
Array<String>
ConvertToS3DArray(const std::vector<std::string>& stdVector)
{
	Array<String> s3dArray;
	for (const auto& stdString : stdVector)
	{
		s3dArray.push_back(Unicode::FromUTF8(stdString));
	}
	return s3dArray;
}


// 小数を含む数値の判定関数
bool is_numeric(const std::string& str)
{
	if (str.empty())
	{
		return false;
	}

	bool has_digit = false;
	bool has_dot = false;

	for (char c : str)
	{
		if (std::isdigit(c))
		{
			has_digit = true;
		}
		else if (c == '.' && !has_dot)
		{
			has_dot = true;
		}
		else
		{
			return false; // 数字とドット以外の文字が含まれている場合
		}
	}

	return has_digit; // 少なくとも1つの数字が含まれている場合
}

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
	Vec2 initialPos{0, 0};

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

namespace
{
	/// @brief 物理演算用に多角形の凸包を計算
	/// @param polygons 多角形
	/// @return 凸包
	Polygon CalculateConvexHull(const MultiPolygon& polygons, const double scale)
	{
		Array<Vec2> points;

		for (const auto& polygon : polygons)
		{
			for (const auto& point : polygon.outer())
			{
				points << (point * scale); // スケールを適用
			}
		}

		return Geometry2D::ConvexHull(points).simplified();
	}
}

/// @brief 各文字を生成
/// @param bottomCenter 最下層の中心位置
/// @param font フォント
/// @param texts 歌詞
/// @param fixed_text 固定文字
/// @return P2Glyph の配列
Array<P2Glyph> GenerateGlyphs(const Vec2& bottomCenter, const Font& font,
                              const Array<String>& texts,
                              const Array<String>& fixed_text)
{
	Array<P2Glyph> all_glyphs;

	// 下から何行目か
	int32 line_count = static_cast<int32>(texts.size());

	// 設定を読み込む
	std::unordered_map<std::string, std::string> settings =
		load_settings(settings_file_path);

	// 比率の初期化
	double kanji_size = 1.0, hiragana_size = 1.0, katakana_size = 1.0;
	double alphabet_upper_size = 1.0, alphabet_lower_size = 1.0, number_size = 1.0;
	double half_width_katakana_size = 1.0, katakana_extensions_size = 1.0;
	std::unordered_map<char32_t, double> option_ratios;

	auto set_ratio = [&](const std::string& key, double& ratio)
	{
		auto it = settings.find(key);
		if (it != settings.end())
		{
			try
			{
				ratio = std::stod(it->second);
			}
			catch (const std::invalid_argument&)
			{
				ratio = 1.0;
			}
		}
	};

	set_ratio("kanji", kanji_size);
	set_ratio("hiragana", hiragana_size);
	set_ratio("katakana", katakana_size);
	set_ratio("alphabet_upper", alphabet_upper_size);
	set_ratio("alphabet_lower", alphabet_lower_size);
	set_ratio("number", number_size);
	set_ratio("half_width_katakana", half_width_katakana_size);
	set_ratio("katakana_phonetic_extensions", katakana_extensions_size);

	// option設定の読み込み
	auto it_option = settings.find("opt_char");
	if (it_option != settings.end())
	{
		std::istringstream iss(it_option->second);
		std::string item;
		while (std::getline(iss, item, ','))
		{
			auto pos = item.find('=');
			if (pos != std::string::npos)
			{
				try
				{
					char32_t code_point = std::stoul(item.substr(0, pos), nullptr, 16);
					double ratio = std::stod(item.substr(pos + 1));
					option_ratios[code_point] = ratio;
				}
				catch (...)
				{
					// 無効なエントリは無視
				}
			}
		}
	}

	auto process_text = [&](const Array<String>& inputTexts,
	                        int32& currentLineCount)
	{
		for (const auto& text : inputTexts)
		{
			const Array<PolygonGlyph> polygonGlyphs = font.renderPolygons(text);
			const Array<double> scales = text.map([&](const char32_t ch)
			{
				if (is_opt_char(ch, option_ratios))
				{
					return option_ratios[ch];
				}
				if (is_kanji(ch))
				{
					return kanji_size;
				}
				if (is_hiragana(ch))
				{
					return hiragana_size;
				}
				if (is_katakana(ch))
				{
					return katakana_size;
				}
				if (is_alphabet_upper_case(ch))
				{
					return alphabet_upper_size;
				}
				if (is_alphabet_lower_case(ch))
				{
					return alphabet_lower_size;
				}
				if (is_number(ch))
				{
					return number_size;
				}
				if (is_half_width_katakana(ch))
				{
					return half_width_katakana_size;
				}
				if (is_katakana_phonetic_extensions(ch))
				{
					return katakana_extensions_size;
				}
				return 1.0;
			});

			Vec2 basePos{0, bottomCenter.y};

			Array<P2Glyph> line;

			for (size_t i = 0; i < polygonGlyphs.size(); ++i)
			{
				const auto& polygonGlyph = polygonGlyphs[i];
				const double scale = scales[i];

				P2Glyph glyph;
				glyph.polygons = polygonGlyph.polygons.scaled(scale);
				glyph.convexHull = CalculateConvexHull(polygonGlyph.polygons, scale);
				glyph.initialPos = (basePos + polygonGlyph.getBase(scale));
				glyph.order = currentLineCount;

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

			all_glyphs.insert(all_glyphs.end(), line.begin(), line.end());

			--currentLineCount;
		}
	};

	process_text(texts, line_count);
	process_text(fixed_text, line_count);

	return all_glyphs;
}


void Main()
{
	// 設定ファイルの存在チェック
	if (!FileSystem::Exists(s3d_settings_file_path))
	{
		System::MessageBoxOK(U"settings.conf が存在しません。", MessageBoxStyle::Error);
		return;
	}

	// 設定を読み込む
	std::unordered_map<std::string, std::string> settings = load_settings(settings_file_path);

	// 設定ファイルの内容を取得
	Array<std::string> texts;
	Array<std::string> fixed_text;
	std::string background_color;
	std::string simulation_speed;
	std::string font_path;
	std::string font_size;

	// 各設定の読み込み
	auto get_setting = [&](const std::string& key, std::string& value,
	                       const std::string& error_message)
	{
		auto it = settings.find(key);
		if (it != settings.end())
		{
			if (it->second.empty())
			{
				System::MessageBoxOK(Unicode::Widen(error_message),
				                     MessageBoxStyle::Error);
				return false;
			}
			value = it->second;
		}
		else
		{
			System::MessageBoxOK(Unicode::Widen(error_message),
			                     MessageBoxStyle::Error);
			return false;
		}
		return true;
	};

	auto get_numeric_setting = [&](const std::string& key,
	                               std::string& value,
	                               const std::string& error_message)
	{
		auto it = settings.find(key);
		if (it != settings.end())
		{
			try
			{
				double tempValue = std::stod(it->second);
				value = std::to_string(tempValue);
			}
			catch (const std::invalid_argument&)
			{
				System::MessageBoxOK(Unicode::Widen(error_message),
				                     MessageBoxStyle::Error);
				return false;
			} catch (const std::out_of_range&)
			{
				System::MessageBoxOK(Unicode::Widen(error_message),
				                     MessageBoxStyle::Error);
				return false;
			}
		}
		else
		{
			System::MessageBoxOK(Unicode::Widen(error_message),
			                     MessageBoxStyle::Error);
			return false;
		}
		return true;
	};


	auto get_array_setting = [&](const std::string& key, Array<std::string>& array,
	                             const std::string& errorMessage)
	{
		auto it = settings.find(key);
		if (it != settings.end())
		{
			array = load_text(it->second);
		}
		else
		{
			System::MessageBoxOK(Unicode::Widen(errorMessage), MessageBoxStyle::Error);
			return false;
		}
		return true;
	};

	// 設定の読み込みと存在チェック
	if (!get_array_setting("lyrics_path", texts, "lyrics_path が設定ファイルにありません。"))
	{
		return;
	}
	if (!get_array_setting("fixed_text_path", fixed_text, "fixed_text_path が設定ファイルにありません。"))
	{
		return;
	}
	if (!get_setting("background_color", background_color,
	                 "background_color が設定ファイルにありません。"))
	{
		return;
	}
	if (!get_setting("simulation_speed", simulation_speed, "simulation_speed が設定ファイルにありません。"))
	{
		return;
	}
	if (!get_setting("font_path", font_path, "font_path が設定ファイルにありません。"))
	{
		return;
	}
	if (!get_setting("font_size", font_size, "font_size が設定ファイルにありません。"))
	{
		return;
	}
	if (!get_numeric_setting("simulation_speed", simulation_speed, "simulation_speed の値が無効です。"))
	{
		return;
	}

	// フルスクリーン設定を確認
	bool full_screen = false;
	auto it_full_screen = settings.find("full_screen");
	if (it_full_screen != settings.end())
	{
		full_screen = (it_full_screen->second == "true");
	}

	if (full_screen)
	{
		// フルスクリーンモードに設定
		Window::SetFullscreen(true);
	}
	else
	{
		int window_width;
		int window_height;

		// 設定から window_width を取得
		auto it_window_width = settings.find("window_width");
		if (it_window_width != settings.end())
		{
			const std::string& width_str = it_window_width->second;

			// 文字列が数値であるかどうかを確認
			if (is_numeric(width_str))
			{
				try
				{
					double temp_width = std::stod(width_str);
					if (temp_width <= 0) // 0以下の値は範囲外とする
					{
						System::MessageBoxOK(
							U"window_width の値は1以上の正の数で指定してください。",
							MessageBoxStyle::Error);
						return;
					}
					window_width = static_cast<int>(temp_width);
				}
				catch (const std::invalid_argument&)
				{
					System::MessageBoxOK(
						U"window_width の値が無効です。数値を指定してください。",
						MessageBoxStyle::Error);
					return;
				} catch (const std::out_of_range&)
				{
					System::MessageBoxOK(U"window_width の値が範囲外です。",
					                     MessageBoxStyle::Error);
					return;
				}
			}
		}
		else
		{
			System::MessageBoxOK(
				U"window_height が設定ファイルにありません。",
				MessageBoxStyle::Error);
			return;
		}

		// 設定から window_height を取得
		auto it_window_height = settings.find("window_height");
		if (it_window_height != settings.end())
		{
			const std::string& height_str = it_window_height->second;

			// 文字列が数値であるかどうかを確認
			if (is_numeric(height_str))
			{
				try
				{
					double temp_height = std::stod(height_str);
					if (temp_height <= 0) // 0以下の値は範囲外とする
					{
						System::MessageBoxOK(
							U"window_height の値は1以上の正の数で指定してください。",
							MessageBoxStyle::Error);
						return;
					}
					window_height = static_cast<int>(temp_height);
				}
				catch (const std::invalid_argument&)
				{
					System::MessageBoxOK(U"window_height の値が無効です。数値を指定してください。",
					                     MessageBoxStyle::Error);
					return;
				} catch (const std::out_of_range&)
				{
					System::MessageBoxOK(
						U"window_height の値が範囲外です。",
						MessageBoxStyle::Error);
					return;
				}
			}
		}
		else
		{
			System::MessageBoxOK(
				U"window_height が設定ファイルにありません。",
				MessageBoxStyle::Error);
			return;
		}

		Window::Resize(window_width, window_height);
	}

	// s3d::Array<s3d::String> に変換
	Array<String> s3d_texts = ConvertToS3DArray(texts);
	Array<String> s3d_fixed_text =
		ConvertToS3DArray(fixed_text);

	// std::stringからs3d::Stringへ変換
	String s3d_font_path = Unicode::FromUTF8(font_path);

	int int_font_size;
	try
	{
		int_font_size = std::stoi(font_size);
	}
	catch (const std::exception)
	{
	}

	// Fontオブジェクトを初期化
	const Font font(int_font_size, s3d_font_path);

	Array<P2Body> body;

	// シミュレーションスピード
	double speed{};
	if (!simulation_speed.empty())
	{
		try
		{
			speed = std::stod(simulation_speed);
		}
		catch (const std::exception)
		{
			speed = 1;
		}
	}

	Window::SetTitle(U"Siv3DGlyphsPhysics_2");
	// 背景
	Scene::SetBackground(
		ColorF{U"#" + Unicode::Widen(background_color)});

	// 2D 物理演算のシミュレーションステップ（秒）
	constexpr double step_time = (1.0 / 200.0);

	// 2D 物理演算のシミュレーション蓄積時間（秒）
	double accumulated_time = 0.0;

	// 基本重力
	constexpr double base_gravity = 160.0;

	// 2D 物理演算のワールド
	P2World world{base_gravity};

	// 画面の幅と高さ
	const double screen_width = Scene::Width();
	const double screen_height = Scene::Height();

	// テキストの幅と高さを計算
	const double text_width = font(s3d_fixed_text).region().w;
	const double text_height = font(s3d_fixed_text).region().h;

	// テキストを画面の中央に配置するための座標を計算
	const Vec2 textPos((screen_width - text_width) / 2, (screen_height - text_height) / 2);

	// 出力されたP2Glyphの配列を処理して物理ワールドに追加
	Array<P2Glyph> glyphs = GenerateGlyphs(Vec2{0, -1100}, font, s3d_texts, Array<String>{});
	Array<P2Glyph> glyph2 =
		GenerateGlyphs(Vec2{0, 0}, font, s3d_fixed_text, Array<String>{});

	for (auto& glyph : glyph2)
	{
		// 物理ボディを物理ワールドに追加
		glyph.body = world.createPolygon(P2Static, glyph.initialPos, glyph.convexHull);
	}

	Camera2D camera{Vec2{0, 0}, 1.0, CameraControl::None_};

	// 何番まで登場しているか
	int32 active_order = 0;

	// 各行の登場タイミングを決めるためのストップウォッチ
	Stopwatch stopwatch{StartImmediately::Yes};

	while (System::Update())
	{
		{
			// 何番まで登場させてよいかを計算
			const int32 t = static_cast<int32>(((stopwatch.ms() * speed) - 800) / 500);

			if (active_order < t) // 順番が進んだ
			{
				++active_order;

				// 同じ順番の文字の物理演算の物体を作成する
				for (auto& glyph : glyphs)
				{
					if (glyph.order == active_order)
					{
						// 文字の物理演算の物体を作成
						glyph.body = world.createPolygon(P2Dynamic, glyph.initialPos, glyph.convexHull);

						// 文字によって重力の倍率を変える
						glyph.body.setGravityScale(Random(1.0, 1.25));

						// 後ろが詰まらないように下向きの初速を与える
						glyph.body.setVelocity(Vec2{0, 90});
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
				glyph.draw(ColorF{0.93});
			}
			for (const auto& glyph : glyph2)
			{
				glyph.draw(ColorF{0.93});
			}
		}

		glyphs.remove_if([&](const P2Glyph& glyph)
		{
			return glyph.body &&
				(glyph.body.getPos().y > (Scene::Height() + 100));
		});

		for (accumulated_time += (Scene::DeltaTime() * speed); step_time <= accumulated_time; accumulated_time -=
		     step_time)
		{
			// 2D 物理演算のワールドを step_time 秒進める
			world.update(step_time);
		}
	}
}
