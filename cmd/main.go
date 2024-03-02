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
	// var wg sync.WaitGroup // for our go routines
	initialTrackRecord := [][]int{
		{maze.Entry[0], maze.Entry[1]},
	}
	fmt.Println("initial track record : ", initialTrackRecord)

	// solve the maze recursivly
	maze.SolveMazeRec(&initialTrackRecord)

	// solve the maze conccurently
	// wg.Add(1)
	// maze.SolveMaze(&wg, &initialTrackRecord)

}
