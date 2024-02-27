package main

import (
	maze "Go-maze-solver/types"
	"fmt"
)

func main() {
	// translate the maze.txt file to a matrix
	maze := &maze.Maze{}
	maze.LoadFromFile("build/dist/maze.txt")

	fmt.Println(maze)

	// some configuration

	// solve the maze
}
