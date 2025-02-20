# Python API の利用を開始する

Python 3 APIを使用すると、独自のスクリプトを作成してカスタムレポートを作成したり、独自のメトリクスを生成したりすることができます。これにより、Understandは自身が求める機能を提供することができます。


Understandには独自の簡略化されたPythonバージョン（upython.exe）がバンドルされているため、APIの使用を簡単に開始できます。スクリプトに「understand」をインポートし、upython実行ファイルを使用してスクリプトを実行するだけです。
スクリプトを実行するには、以下のコマンドを使用します：
```
c:\Program Files\SciTools\pc-win64>upython
>>> import understand
>>> understand.version()
1107
>>>
```
## ドキュメント
最新のドキュメントはUnderstandのヘルプメニューからアクセスできます。[ヘルプ] -> [Python API ヘルプ]を選択してください。

## サンプルスクリプト
これらのスクリプトに加えて、ヘルプにはいくつかのサンプルも含まれています。さらに詳しいサンプルスクリプトは、Understandに同梱されているSciTools/plugins/Scripts/Pythonフォルダーにあります。

# カスタムPythonインストール
カスタムPythonライブラリを使用したい場合は、以下の手順に従ってUnderstandを自分のPythonインストールに追加する必要があります:

1. Python APIを使用するには、Python 3.xがインストールされている必要があります。また、Pythonのビット数はUnderstandと同じでなければなりません。つまり、Understandが64ビット版であればPython 3も64ビット版である必要があります。
1. PYTHONPATH環境変数を変更して、モジュールの場所（通常はC:\Program Files\SciTools\bin\pc-win64\Python）を含めるようにします。以下の手順で行います。
1. PATH環境変数を C:\Program Files\SciTools\bin\pc-win64 に設定します。
1. Linuxのみ: LD_LIBRARY_PATH環境変数を scitools/bin/linux64 に設定します。
1. APIをテストするには、Understandに同梱されているテストスクリプトを実行します:
1. python “C:\Program Files\SciTools\plugins\Scripts\Python\apiInstallTest.py”
1. スクリプト内に「import understand」を追加します。
1. Python 3.8以降では、スクリプト内でos.add_dll_directoryコマンドを使用してUnderstandのインストールパスを指定する必要があります。以下の例を参照してください:

```
c:\python39>python
>>> import os
>>> os.add_dll_directory("C:\Program Files\Scitools\bin\pc-win64")
>>> import understand
>>> understand.version()
1055
>>>
```

### Cygwin
残念ながら、CygwinのPythonは、LinuxとWindowsの両方の環境を独自の方法でリンクするため、サポートすることができません。

## トラブルシューティング
python.orgから取得できる公式のPythonディストリビューションには安定したABIを実装したpython3.dllが含まれています。これにより、同じモジュールでPython 3.2以降をターゲットにすることができます。
本質的にはpython3x.dllに転送するラッパーです。Anacondaのような他のPythonディストリビューションにはpython3.dllが含まれていない場合がありますが、公式のディストリビューションからコピーすることができる可能性があります。