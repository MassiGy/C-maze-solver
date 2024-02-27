package maze

import (
	"io"
	"os"
	"strconv"
	"sync"
)

const (
	WALL = iota
	STEP
)
const (
	EntryVal = 2
	EndVal   = 3
)

type Maze struct {
	Entry [2]int
	End   [2]int

	LineCount int
	ColCount  int

	Grid [][]int
}

// @todo: continue this function
func (maze Maze) SolveMaze(wg *sync.WaitGroup, trackRecord [][]int) {
	defer wg.Done()

	currentPoint := trackRecord[len(trackRecord)-1]

	if maze.Grid[currentPoint[0]][currentPoint[1]] == WALL {
		// just exit (this will be unregistred from wg)
		return
	} else if maze.Grid[currentPoint[0]][currentPoint[1]] == EndVal {
		// we win, exit
		return
	}

	goingUpTrackRecord := append(trackRecord, []int{currentPoint[0] - 1, currentPoint[1]})
	goingDownTrackRecord := append(trackRecord, []int{currentPoint[0] + 1, currentPoint[1]})
	goingLeftTrackRecord := append(trackRecord, []int{currentPoint[0], currentPoint[1] - 1})
	goingRightTrackRecord := append(trackRecord, []int{currentPoint[0], currentPoint[1] + 1})

	if maze.canGoUp(currentPoint[0], currentPoint[1]) {
		wg.Add(1)
		go maze.SolveMaze(wg, goingUpTrackRecord)

	} else if maze.canGoDown(currentPoint[0], currentPoint[1]) {
		wg.Add(1)
		go maze.SolveMaze(wg, goingDownTrackRecord)

	} else if maze.canGoLeft(currentPoint[0], currentPoint[1]) {
		wg.Add(1)
		go maze.SolveMaze(wg, goingLeftTrackRecord)

	} else if maze.canGoRight(currentPoint[0], currentPoint[1]) {
		wg.Add(1)
		go maze.SolveMaze(wg, goingRightTrackRecord)
	}

	// wait for go routines to end
	wg.Wait()

	// check which track record contains the end
}

func (maze *Maze) LoadFromFile(filename string) error {

	file, err := os.Open(filename)

	if err != nil {
		return err
	}

	// since the file won't be used again after this fn call
	defer file.Close()

	maze.Grid = make([][]int, 0)

	buf := make([]byte, 10)

	line := 0
	col := 0

	maze.Grid = append(maze.Grid, []int{})

	for {
		_, err := file.Read(buf)

		if err == io.EOF {
			break
		}

		for _, b := range buf {
			if b == byte('\n') {
				line++
				// add a new slice for the new line
				maze.Grid = append(maze.Grid, []int{})
				continue
			}
			intVal, err := strconv.Atoi(string(b))
			if err != nil {
				// this means that we've reached end of file
				break
			}

			// add in a new item in the new colomn
			maze.Grid[line] = append(maze.Grid[line], intVal)

			if int(b) == EntryVal {
				maze.Entry[0] = line
				maze.Entry[1] = col
			}
			if int(b) == EndVal {
				maze.End[0] = line
				maze.End[1] = col
			}

			col++
		}
	}
	return nil
}

func (maze Maze) canGoUp(lineIndex, colIndex int) bool {
	return lineIndex-1 >= 0 && maze.Grid[lineIndex-1][colIndex] != WALL
}

func (maze Maze) canGoDown(lineIndex, colIndex int) bool {
	return lineIndex+1 < maze.LineCount && maze.Grid[lineIndex+1][colIndex] != WALL
}

func (maze Maze) canGoLeft(lineIndex, colIndex int) bool {
	return colIndex-1 >= 0 && maze.Grid[lineIndex][colIndex-1] != WALL
}
func (maze Maze) canGoRight(lineIndex, colIndex int) bool {
	return colIndex+1 < maze.ColCount && maze.Grid[lineIndex][colIndex+1] != WALL
}
