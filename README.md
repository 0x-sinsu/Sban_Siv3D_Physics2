# Sban_Siv3D_Physics2  
  
## これ何？  
[Sban_Siv3D_Physics1](https://github.com/0x-sinsu/Sban_Siv3D_Physics1)  
とセット。あちらは[本家動画](https://www.youtube.com/watch?v=7CUpc5K1li4)の1:36辺りの部分の物理演算です。  
一方、こちらは3:10辺りの部分の物理演算を再現しています。  
  
## 使い方  
まず、重要なことを先に。  

### このプログラムはフルスクリーンで実行されます。  
### 「Esc」キーを押すことでプログラムは終了します。  

知らずにどうやって直るの！？となってしまってはいけませんからね。  
  
Releaseから`Sban_Siv3D_Physics2_{バージョン}.zip`をダウンロードし、展開します。  
バージョン部分は取り敢えずYYYYMMDD形式です。v0.1とかは面倒だったので。  
Zipへのバージョン表記不要説はある。  
展開したフォルダの中身は次のような感じになっているはずです。  
  
```
Sban_Siv3D_Physics2_{バージョン}
  LSbanPhysics2.exe
  Lfixed.txt
  Llyrics.txt
  LGenZenGothicKaiK.ttf
```

各ファイルの説明をします。  
  
    SbanPhysics2.exe - 本体  
    settings.conf - 設定ファイル  
    fixed.txt - 画面中央に固定する文字列を指定するファイル  
    lyrics.txt - 落としたい文字列を指定するファイル  
    GenZenGothic改.ttf - 不具合を取り敢えず対処する用のフォント  
  
GenZenGothic改.ttfはK様の作成されたフォントです。  
SIL Open Font Licenseです。  
仮名のサイズが漢字に対し76~75%に設定されています。  
本来は当ソフトに仮名の比率を調節する機能があるのですが、めり込むなどの不具合があるためこちらをお使い頂くことを推奨致します。  

settings.confについては、`fontSize=`の部分だけ編集すればOKです。  
名前の通りフォントサイズを指定します。  
UTF-8、改行コードはCR+LFで動作することを確認しています。  
fixed.txt、lyrics.txtにおいても同様です。


ちなみに、大体のコードはS3Dコミュの方とかAIとかに書いてもらいました  
当の本人はC++初心者です  
  
  
## ソースからビルドする  
slnファイルを開いてx64、Releaseにしてビルドします。  
x64 Native Tools~みたいなやつでやるなら、
```
SbanPhysics2.sln /p:Configuration=Release /p:Platform="x64"
```
になります。
