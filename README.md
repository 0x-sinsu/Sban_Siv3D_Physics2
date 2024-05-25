# Sban_Siv3D_Physics2  
  
## 概要  
[Sban_Siv3D_Physics1](https://github.com/0x-sinsu/Sban_Siv3D_Physics1)  
とセット。あちらは[本家動画](https://www.youtube.com/watch?v=7CUpc5K1li4)の1:36辺りの部分の物理演算です。  
一方、こちらは3:10辺りの部分の物理演算を再現しています。  
  
## 使い方  
### このプログラムはフルスクリーンで実行されます。  
### 「Esc」キーを押すことでプログラムは終了します。  
<br><br>
  
Releaseから`Sban_Siv3D_Physics2_{バージョン}.zip`をダウンロードし、展開します。<br><br>
バージョン部分はYYYYMMDD形式です。  
展開したフォルダの中身は次のような感じになっているはずです。  
  
    Sban_Siv3D_Physics2_{バージョン}
      LSbanPhysics2.exe
      Lfixed.txt
      Llyrics.txt
      LGenZenGothicKaiK.ttf

各ファイルの説明をします。  
  
    SbanPhysics2.exe - 本体  
    settings.conf - 設定ファイル  
    fixed.txt - 画面中央に固定する文字列を指定するファイル  
    lyrics.txt - 落としたい文字列を指定するファイル  
    GenZenGothic改.ttf - 不具合を取り敢えず対処する用のフォント  
  
GenZenGothic改.ttfはK様によって作成されたフォントです。  
仮名のサイズが漢字に対し76~75%に設定されています。  
本来は当ソフトに文字サイズの比率を調節する機能があるのですが、不具合があるためこちらをお使い頂くことを推奨致します。  

settings.confは、<br><br>
`simulationSpeed=`...スピードを指定する  
`frameRate=`...フレームレートを指定する  
`fontSize=`...フォントサイズを指定する  
`fontPath=`...フォントパスを指定する<br><br>
といった感じで値を指定しておきます。  
UTF-8、改行コードはCR+LFで動作することを確認しています。  
fixed.txt、lyrics.txtにおいても同様、UTF-8、CR+LFで動作します。  
(恐らくですが、改行コードはCR+LFでなくても問題ないはずです)


ちなみに、大体のコードはS3Dコミュの方とかAIとかに書いてもらいました  
当の本人はC++初心者です  
  
  
## ビルドする  
OpenSiv3DのSDKが必要です。  
[ここ](https://siv3d.github.io/ja-jp/)を見て導入してください。  
Windows、mac、Ubuntuの3つがありますが、私がビルドできることを確認しているのはWindowsのみです。  
OSに依存するコードはないので特に問題なくビルドできると思いますが...<br><br>
開発環境はWindows 11 Pro、Visual Studio Community 2022です。<br><br>
ビルドすると、  
`Intermediate/SbanPhysics2/Release`  
もしくは  
`Intermediate/SbanPhysics2/Debug`  
に実行ファイルなどが集まります。
