# Sban_Siv3D_Physics2  


※ビルドしなくても使えるように改良中...あと4E00~以外の漢字への対応もするかもしれない...  
  
## これ何？  
[Sban_Siv3D_Physics1](https://github.com/0x-sinsu/Sban_Siv3D_Physics1)  
とセット。あちらは[本家動画](https://www.youtube.com/watch?v=7CUpc5K1li4)の1:36辺りの部分の物理演算です。  
一方、こちらは3:10辺りの部分の物理演算を再現しています。  
基本的な使い方は同じです。  
多分動く...はずです。
## 使い方  
Build Tools for Visual Studio 2022のインストール、OpenSiv3Dのインストールは[こちら](https://github.com/0x-sinsu/Sban_Siv3D_Physics1/blob/main/README.md)の方法と同じです。  
ここでは割愛させていただきます。  
  
### ソースコードのダウンロード、配置  
次に、[こちら](https://github.com/0x-sinsu/Sban_Siv3D_Physics1)のページの「Code」をクリック、「Download ZIP」をクリックしてダウンロードします。  
(gitでも問題なし)  
任意のフォルダに配置します。  
次に、lyrics.txtに上から降らせる歌詞を入力し、保存します。(文字エンコーディングはUTF-8)  
次に、fixed.txtに画面中央に固定する文を入力し、保存します。(こちらもUTF-8)  
次にSbanPhysics2/edit.exeを実行する  
↓誤判定された場合のみで良いと思います  
(のですが、現在ウイルスと誤判定されてしまうようですので[こちら](https://support.microsoft.com/ja-jp/windows/windows-%E3%82%BB%E3%82%AD%E3%83%A5%E3%83%AA%E3%83%86%E3%82%A3%E3%81%AB%E3%82%88%E3%82%8B%E4%BF%9D%E8%AD%B7%E3%82%92%E5%88%A9%E7%94%A8%E3%81%97%E3%81%BE%E3%81%99-2ae0363d-0ada-c064-8b56-6a39afb6a963)のサイトの下の方に記載されている  
"リアルタイム保護を一時的に無効にする"  
の手順に従い無効化します。  
作業が終了したら必ず元に戻します。なお、他のセキュリティソフトに関しては私は分かりませんので各自お調べください。  
ウイルス入ってるんじゃ...と思った場合はedit.cppの中身をチェックしてください。  
それでも不安な場合はedit.cppを各自コンパイルしてください。  
コンパイル方法は割愛します。  )  
edit.exeを開いたら、指示に従い数値を入力します。  
(標準値というのはあくまでも私がテストした時点でいい感じだった値ですので参考程度にしてください)  
この後のbuild.batを実行する際もリアルタイム保護の無効化が必要なため、まだ無効化のままにしておきます。  
  
### ビルド
build.batを実行します。  
しばらく待つとコマンドプロンプトが閉じますので、そうなったらビルド完了です。  
ビルドが終了すると、Intermediate\SbanPhysics2\Release内にSbanPhysics1.exeファイルが出来ています。  
ここでリアルタイム保護を有効に戻します。  
  
---以下は以前のビルド方法です---  
次にスタートメニューで「x64 Native Tools Command Prompt for VS 2022」と検索し、出てきたものを実行します。  
そこに以下のコマンドを貼り付け、実行します。  
```Batchfile
msbuild "先ほど控えたフォルダパス\SbanPhysics2.sln" /p:Configuration=Release /p:Platform=x64
```
大量の文が流れてきますが、そのまま入力待機状態になるまで待ちます。  
ビルドが終了すると、Intermediate\Physics2\Release内にexeファイルが出来ています。  
  
### このプログラムはフルスクリーンで実行されます。  
### 「Esc」キーを押すことでプログラムは終了します。  
実行される画面のサイズ変更方法は1のreadmeに書いています。  

vswhereを使用しています:https://github.com/microsoft/vswhere
