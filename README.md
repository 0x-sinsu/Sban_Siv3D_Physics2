# Siv3DGlyphsPhysics_2  
  
## 概要  
[Siv3DGlyphsPhysics_1](https://github.com/0x-sinsu/Siv3DGlyphsPhysics_1)  
とセット。あちらは[本家動画](https://www.youtube.com/watch?v=7CUpc5K1li4)の1:36辺りの部分の物理演算です。  
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
バージョンによって中身が違うことがありますが。  
#### ファイルの説明  
    SbanPhysics2.exe - 本体  
    settings.conf - 設定ファイル  
    fixed.txt - 画面中央に固定する文字列を指定するファイル  
    lyrics.txt - 落としたい文字列を指定するファイル  
(後日追記)
  
#### 設定する  
settings.confは、<br><br>
`simulationSpeed=`...スピードを指定する  
`frameRate=`...フレームレートを指定する  
`fontSize=`...フォントサイズを指定する  
`fontPath=`...フォントパスを指定する<br><br>
といった感じで値を指定しておきます。  
UTF-8で保存します。(lyrics.txt,fixed.txtも同様)
(後日追記)
  
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
