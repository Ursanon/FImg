# genetic-drawer

===

# demo
| Generation: | 100 ~6 [s]  | 200  | 500  | 1000 | 2000 | 3700 ~5 [min] |
|:-:|:-:|:-:|:-:|:-:|:-:|:-:|
|   | ![gen 100](resources/100.png)  | ![gen 200](resources/200.png) | ![gen 500](resources/500.png) | ![gen 1000](resources/1000.png) | ![gen 2000](resources/2000.png) | ![gen 3700](resources/3700.png) |

## generated with
`①` = `-g`
`④` = `330`
`⑤` = `2`

# run commands
Run with `-h` or `--help` to see simple tutorial

`./genetic-drawer.exe ① ② ③ ④ ⑤ ⑥ ⑦ ⑧ ⑨`
where
`①` - image format: `-rgb` (24BPP) / `-g` (8BPP)
`②` - path to target image
`③` - path to output directory
`④` - specimens count (> 0)
`⑤` - parents count (> 0)
`⑥` - save interval (> 0)
`⑦` - image's width
`⑧` - image's height
`⑨` - threads count

# cli compilation with g++:
`g++ -std=c++11 -Iinclude/ src/*.cpp`