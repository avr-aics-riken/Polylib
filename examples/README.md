
# Polylib Examples

## How to run

~~~
$ cp data_bck/* .; mpirun -np 4 ./test_mpi2
~~~


## Description

- `test`
  - インスタンス、コンフィグファイルのロード、表示、stlファイル出力のテスト


- `test2`
  - 複数オブジェクトのロードテスト


- `test_mpi`
  - testの並列テスト


- `test_mpi_xyzrgb*`
  - 並列度の違いによる時間測定用プログラム


- `test_DVertex`
  - データ付頂点クラスのテストプログラム


- `test_mpi_DVertex`
  - データ付頂点クラスのmpi版テストプログラム


- `test_obj`
  - objファイルアスキーを読み込むプログラム
  - 出力はOBJ_BIN(法線なし)


- `test_obj2`
  - objファイルアスキーを読み込むプログラム
  - 出力はOBJ_BIN(法線あり)


- `test_obj3`
  - objファイルアスキーを読み込むプログラム
  - 出力はOBJアスキー


- `test_obj4`
  - objファイルバイナリを読み込むプログラム
  - 出力はOBJアスキー


- `test_vtx`
  - test_vtx_float
  - 頂点クラスの確認用プログラム
