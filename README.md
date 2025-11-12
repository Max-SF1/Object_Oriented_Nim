# Object Based Nim 

This is a **console-based implementation of the classic Nim game** written in C++. It supports both human players and AI opponents, including a **random-move bot** and a **strategically optimal bot**.  

The game allows custom board setups and provides a clean, visual representation of the current board state in the console.

---

## Features

- **Customizable board**: Players can define the number of rows and objects per row. Defaults to `[3, 5, 7]`.
- **Multiple player types**:
  - Human player
  - Random-move AI bot
  - Strategically optimal AI bot (implements the Nim optimal strategy using XOR/Nim sum)
- **Turn-based gameplay** with automatic validation of moves.
- **Interactive console interface** with board display using stars for objects.
- **1v1 or human vs bot** gameplay.

---

## How to Play

1. Run the program:  
   ```bash
   ./main
