# wasmCheerpSnake
install Cheerp compile;
/opt/cheerp/bin/clang++ -target cheerp -cheerp-mode=wasm -cheerp-wasm-loader=pong.js -O3 -o snake.wasm snake.cpp
