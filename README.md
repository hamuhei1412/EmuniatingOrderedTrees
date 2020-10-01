# EmuniatingOrderedTrees
## 概略 
このリポジトリでは、第113回SIG-FPAIに投稿された「フロンティア法を用いた次数制約付き順序木列挙」という論文内で述べられている手法の著者実装・実験データが管理されている。

以下を次数制約付き順序木列挙問題と定め、解く。

入力:自然数n, k  
出力:頂点数nで、次数がk以下の頂点のみで構成される順序木を同型なものを除いて過不足なく表現するZDD

動作には、g++とboostが必要である。
また、フロンティア法を実装するためのライブラリである、[TdZDD](https://github.com/kunisura/TdZdd)を用いている。

コードの中で、TdZDDに対するパスは、このディレクトリ(EmuniatingOrderedTrees)と同じ階層にあると仮定している。(相対パスで指定している。)

また、TdZddの最新バージョンでは動作が不安定であることが確認されている。(2020年9月現在)

TdZddのリポジトリの2020年6月のコミット時点のものでは動作が確認できているので、そちらを使うことが推奨される。

src/build_zdd.cppをg++でビルドして、吐かれたバイナリに二つの整数n, kを渡すと、頂点数n、次数制約kであるようなZDDを構築する。

## 実験

experimentディレクトリに実験にて取得したデータが入っている。
実験には、Ubuntu 18.04 LTS, メモリ容量16GB, CPU AMDRyzen 5 3600の計算機を用いた。

実験データは、いくつかのテキストファイルで表され、テキストファイルの名前の形式は、k=a_xxx.txtのような形で表される。
k=aは、次数制約がaであることを表している。特に、k=n-1は次数制約がないことを表している。

テキストファイルには、1 ~ 150行目にデータが記載されていて、i行目のデータは、頂点数がiであることを表している。

_xxxの部分は、採取したデータの種類を表している。
それぞれ、answerは解(順序木)の個数、zdd_sizeはzddの大きさ、compressibilityは圧縮率(= ZDDの幅/解の個数)、build_timeが構築時間(単位は[s])を表している。

例えば、k=2_answer.txtには、n行目に、「頂点数n、次数制約2の順序木の個数」がプリントされている。

特に、answerにはk=2 ~ 10, n-1について実験が行われていて、これらは手法の正当性について検証するものである。  
次数制約がない場合は、カタラン数に一致することが確認できる。  
k=2の場合は順序木の個数はモツキン数となり、一致することが確認できた。  
k=3 ~ 10の場合については、[oeis](https://oeis.org/)にこの問題設定と同等の問題設定のページがあり([A036765](https://oeis.org/A036765)等)、一致することが確認できる。


