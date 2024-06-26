# Siv3DGlyphsPhysics_2  
  
## 概要  
[Siv3DGlyphsPhysics_1](https://github.com/0x-sinsu/Siv3DGlyphsPhysics_1)  
とセット。あちらは[.. - 全てあなたの所為です。](https://www.youtube.com/watch?v=7CUpc5K1li4)の1:36辺りの部分の物理演算です。  
一方、こちらは3:10辺りの部分の物理演算を再現しています。  
あちらと機能が違う(こちらの方が多い)ですが、同時に進めるのは無理ですのでとりあえずこちらをある程度開発してからもう一つの方も進めようかなと思っています。  
余談:大体のコードはS3Dコミュの方とかAIとかに書いてもらいました  当の本人はC++初心者です(えっ)  
  
## 使い方  
ReleaseからLatestなZipを落として展開します。  
展開したフォルダの中身は次のような感じになっているはずです。  
  
    展開フォルダ
      LSiv3DGlyphsPhysics_2.exe
      Lsettings.conf
      Lfixed.txt
      Llyrics.txt
バージョンによって中身が違うことがありますが、だいたい同じです。大きく異なるときはReadmeを編集するので...  
#### ファイルの説明  
    SbanPhysics2.exe - 本体  
    settings.conf - 設定ファイル  
    fixed.txt - 画面中央に固定する文字列を指定するファイル  
    lyrics.txt - 落としたい文字列を指定するファイル  
(後日追記)
  
#### 設定する  
settings.confは次のように設定します。<br><br>
```
lyrics_path=./lyrics.txt(変えない)
fixed_text_path=./fixed.txt(変えない)
simulation_speed=(スピードを指定、1.75など)
window_width=(ウィンドウの横幅を指定、1600など)
window_height=(ウィンドウの縦幅を指定、900など)
background_color=(背景の色を指定、000000など6桁のカラーコードを指定する)
texts_color=(降ってくる文字の色を指定、background_colorと同様)
fixed_color=(中央に固定される文字の色を指定、background_colorと同様)
font_size=(フォントサイズを指定、75など)
font_path=(フォントパスを指定)

(以下はフォントサイズの比率を指定。font_sizeで指定したものが1となる)
kanji=
hiragana=
katakana=
alphabet_upper=
alphabet_lower=
number=
half_width_katakana=
katakana_phonetic_extensions=
opt_char=(ここでは特定の文字の比率を変えることができる。以下に記述)
```

opt_charの使い方:例えば「全」(U+5168)の字の比率を5.5に、「て」(U+3066)の字の比率を10.0にしたい場合、  
`opt_char=0x5168=5.5,0x3066=10.0`  
となります。  

ファイルはUTF-8で保存します。(lyrics.txt,fixed.txtも)  
  
## ソースからビルド  
OpenSiv3DのSDKが必要です。  
[ここ](https://siv3d.github.io/ja-jp/)を見て導入してください。  
開発環境はWindows 11 Pro、Visual Studio Community 2022です。<br><br>
ビルドすると、  
`Buikd/Release`  
もしくは  
`Build/Debug`  
に実行ファイルなどが集まります。<br><br>
ローカルWindowsデバッガーを使うときは  
`Siv3DGlyphsPhysics_2/App`  
内にexeが生成されるので、そこに設定ファイルなどを置いておきます。
