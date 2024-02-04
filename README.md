# Sban_Siv3D_Physics2(準備中、使用不可)  
## これ何？  
[Sban_Siv3D_Physics1](https://github.com/0x-sinsu/Sban_Siv3D_Physics1)  
とセット。あちらは[本家動画](https://www.youtube.com/watch?v=7CUpc5K1li4)の1:36辺りの部分の物理演算です。  
一方、こちらは3:10辺りの部分の物理演算を再現しています。  
基本的な使い方は同じです。  
## 使い方  
Build Tools for Visual Studio 2022のインストール、OpenSiv3Dのインストールは[こちら](https://github.com/0x-sinsu/Sban_Siv3D_Physics1/blob/main/README.md)の方法と同じです。  
ここでは割愛させていただきます。  
  
### ソースコードのダウンロード、配置  
次に@、[こちら](https://github.com/0x-sinsu/Sban_Siv3D_Physics2)のページの「Code」をクリック、「Download ZIP」をクリックしてダウンロードします。  
(gitでも問題なし)  
任意のフォルダに配置します。  
Physics2.slnがあるフォルダのパスを控えておきます。  
次に、Main.cppをVSCodeなどで開きます。 編集する箇所をコメントアウトで示していますので、それを参考に編集してください。  
編集する部分の大まかな行は、 77行目 118行目 127行目 146行目 172行目 231行目 334行目 辺りです。  
分からない所があればTwitterのDMやDiscordまでどうぞ。  
Twitter:[@0x_sinsu](https://twitter.com/0x_sinsu)  
Discord:subete_light  
  
### ビルド
次にスタートメニューで「x64 Native Tools Command Prompt for VS 2022」と検索し、出てきたものを実行します。  
そこに以下のコマンドを貼り付け、実行します。  
```Batchfile
msbuild "先ほど控えたフォルダパス\Physics2.sln" /p:Configuration=Release /p:Platform=x64
```
大量の文が流れてきますが、そのまま入力待機状態になるまで待ちます。  
ビルドが終了すると、Intermediate\Physics2\Release内にexeファイルが出来ています。  
### このプログラムはフルスクリーンで実行されます。  
### 「Esc」キーを押すことでプログラムは終了します。  
実行される画面のサイズ変更方法などはいずれ追記します。変更するのであれば意図する動作になるようにご自由にコードを書き換えてください。  
