package maze

import (
	"fmt"
	"os"
	"strconv"
	"strings"
	"sync"
)

const (
	WALL     int = 0
	STEP     int = 1
	EntryVal int = 2
	EndVal   int = 3
)

type Maze struct {
	Entry [2]int
	End   [2]int

	LineCount int
	ColCount  int

	Grid [][]int
}

func (maze Maze) SolveMazeRec(trackRecord *[][]int) {

	currentPoint := (*trackRecord)[len(*trackRecord)-1]

	// @todo: change the exit signaling into a channel
	if maze.Grid[currentPoint[0]][currentPoint[1]] == WALL {
		fmt.Println("Hit a wall, exiting....")
		return

	} else if maze.Grid[currentPoint[0]][currentPoint[1]] == EndVal {
		fmt.Println("Found the end, exiting....")
		return

	} else if maze.Grid[currentPoint[0]][currentPoint[1]] == EntryVal && len(*trackRecord) > 1 {
		fmt.Println("Circled back to the start , exiting....")
		return
	}

	var goingUpTrackRecord [][]int
	var goingDownTrackRecord [][]int
	var goingLeftTrackRecord [][]int
	var goingRightTrackRecord [][]int

	exploreUp := maze.canGoUpFrom(currentPoint[0], currentPoint[1]) && !trackContains(*trackRecord, []int{currentPoint[0] - 1, currentPoint[1]})
	exploreDown := maze.canGoDownFrom(currentPoint[0], currentPoint[1]) && !trackContains(*trackRecord, []int{currentPoint[0] + 1, currentPoint[1]})
	exploreLeft := maze.canGoLeftFrom(currentPoint[0], currentPoint[1]) && !trackContains(*trackRecord, []int{currentPoint[0], currentPoint[1] - 1})
	exploreRight := maze.canGoRightFrom(currentPoint[0], currentPoint[1]) && !trackContains(*trackRecord, []int{currentPoint[0], currentPoint[1] + 1})

	if exploreUp {

		goingUpTrackRecord = append(*trackRecord, []int{currentPoint[0] - 1, currentPoint[1]})
		// fmt.Println("can go up: goingUpTrackRecord: ", goingUpTrackRecord)
		maze.SolveMazeRec(&goingUpTrackRecord)

	}
	if exploreDown {
		goingDownTrackRecord = append(*trackRecord, []int{currentPoint[0] + 1, currentPoint[1]})
		// fmt.Println("can go down: goingDownTrackRecord: ", goingDownTrackRecord)
		maze.SolveMazeRec(&goingDownTrackRecord)

	}
	if exploreLeft {
		goingLeftTrackRecord = append(*trackRecord, []int{currentPoint[0], currentPoint[1] - 1})
		// fmt.Println("can go left: goingLeftTrackRecrod: ", goingLeftTrackRecord)
		maze.SolveMazeRec(&goingLeftTrackRecord)

	}
	if exploreRight {
		goingRightTrackRecord = append(*trackRecord, []int{currentPoint[0], currentPoint[1] + 1})
		// fmt.Println("can go right: goingRightTrackRecord: ", goingRightTrackRecord)
		maze.SolveMazeRec(&goingRightTrackRecord)
	}

	var point []int
	// check which track record contains the end
	if exploreUp {

		point = goingUpTrackRecord[len(goingUpTrackRecord)-1]
		if point[0] == maze.End[0] && point[1] == maze.End[1] {
			fmt.Println("Go routine found the end, exiting...")
			fmt.Println("track record: ", goingUpTrackRecord)
			return
		}

	}
	if exploreDown {

		point = goingDownTrackRecord[len(goingDownTrackRecord)-1]
		if point[0] == maze.End[0] && point[1] == maze.End[1] {
			fmt.Println("Go routine found the end, exiting...")
			fmt.Println("track record: ", goingDownTrackRecord)
			return
		}

	}
	if exploreLeft {

		point = goingLeftTrackRecord[len(goingLeftTrackRecord)-1]
		if point[0] == maze.End[0] && point[1] == maze.End[1] {
			fmt.Println("Go routine found the end, exiting...")
			fmt.Println("track record: ", goingLeftTrackRecord)
			return
		}

	}
	if exploreRight {

		point = goingRightTrackRecord[len(goingRightTrackRecord)-1]
		if point[0] == maze.End[0] && point[1] == maze.End[1] {
			fmt.Println("Go routine found the end, exiting...")
			fmt.Println("track record: ", goingRightTrackRecord)
			return
		}

	}

}

func (maze Maze) SolveMaze(parent_wg *sync.WaitGroup, trackRecord *[][]int) {
	defer parent_wg.Done()

	currentPoint := (*trackRecord)[len(*trackRecord)-1]

	// @todo: change the exit signaling into a channel
	if maze.Grid[currentPoint[0]][currentPoint[1]] == WALL {
		fmt.Println("Hit a wall, exiting....")
		return

	} else if maze.Grid[currentPoint[0]][currentPoint[1]] == EndVal {
		fmt.Println("Found the end, exiting....")
		return

	} else if maze.Grid[currentPoint[0]][currentPoint[1]] == EntryVal && len(*trackRecord) > 1 {
		fmt.Println("Circled back to the start , exiting....")
		return
	}

	var goingUpTrackRecord [][]int
	var goingDownTrackRecord [][]int
	var goingLeftTrackRecord [][]int
	var goingRightTrackRecord [][]int

	exploreUp := maze.canGoUpFrom(currentPoint[0], currentPoint[1]) && !trackContains(*trackRecord, []int{currentPoint[0] - 1, currentPoint[1]})
	exploreDown := maze.canGoDownFrom(currentPoint[0], currentPoint[1]) && !trackContains(*trackRecord, []int{currentPoint[0] + 1, currentPoint[1]})
	exploreLeft := maze.canGoLeftFrom(currentPoint[0], currentPoint[1]) && !trackContains(*trackRecord, []int{currentPoint[0], currentPoint[1] - 1})
	exploreRight := maze.canGoRightFrom(currentPoint[0], currentPoint[1]) && !trackContains(*trackRecord, []int{currentPoint[0], currentPoint[1] + 1})

	var current_wg sync.WaitGroup

	if exploreUp {
		current_wg.Add(1)
		goingUpTrackRecord = append(*trackRecord, []int{currentPoint[0] - 1, currentPoint[1]})
		fmt.Println("can go up: goingUpTrackRecord: ", goingUpTrackRecord)
		go maze.SolveMaze(&current_wg, &goingUpTrackRecord)

	}
	if exploreDown {
		current_wg.Add(1)
		goingDownTrackRecord = append(*trackRecord, []int{currentPoint[0] + 1, currentPoint[1]})
		fmt.Println("can go down: goingDownTrackRecord: ", goingDownTrackRecord)
		go maze.SolveMaze(&current_wg, &goingDownTrackRecord)

	}
	if exploreLeft {
		current_wg.Add(1)
		goingLeftTrackRecord = append(*trackRecord, []int{currentPoint[0], currentPoint[1] - 1})
		fmt.Println("can go left: goingLeftTrackRecrod: ", goingLeftTrackRecord)
		go maze.SolveMaze(&current_wg, &goingLeftTrackRecord)

	}
	if exploreRight {
		current_wg.Add(1)
		goingRightTrackRecord = append(*trackRecord, []int{currentPoint[0], currentPoint[1] + 1})
		fmt.Println("can go right: goingRightTrackRecord: ", goingRightTrackRecord)
		go maze.SolveMaze(&current_wg, &goingRightTrackRecord)
	}

	// wait for go routines to end
	current_wg.Wait()

	var point []int

	// check which track record contains the end
	if exploreUp {

		// @todo: consider walking through the track record instead
		point = goingUpTrackRecord[len(goingUpTrackRecord)-1]
		if point[0] == maze.End[0] && point[1] == maze.End[1] {
			fmt.Println("Go routine found the end, exiting...")
			fmt.Println("track record: ", goingUpTrackRecord)
			return
		}

	}
	if exploreDown {

		point = goingDownTrackRecord[len(goingDownTrackRecord)-1]
		if point[0] == maze.End[0] && point[1] == maze.End[1] {
			fmt.Println("Go routine found the end, exiting...")
			fmt.Println("track record: ", goingDownTrackRecord)
			return
		}

	}
	if exploreLeft {

		point = goingLeftTrackRecord[len(goingLeftTrackRecord)-1]
		if point[0] == maze.End[0] && point[1] == maze.End[1] {
			fmt.Println("Go routine found the end, exiting...")
			fmt.Println("track record: ", goingLeftTrackRecord)
			return
		}

	}
	if exploreRight {

		point = goingRightTrackRecord[len(goingRightTrackRecord)-1]
		if point[0] == maze.End[0] && point[1] == maze.End[1] {
			fmt.Println("Go routine found the end, exiting...")
			fmt.Println("track record: ", goingRightTrackRecord)
			return
		}

	}

}

func (maze *Maze) LoadFromFile(filename string) error {
	fileContent, err := os.ReadFile(filename)
	check(err)

	// convert the fileContent from bytes to string
	fileContentAsStr := string(fileContent)

	// split by \n to get the lines
	lines := strings.Split(fileContentAsStr, "\n")

	maze.LineCount = len(lines)
	maze.ColCount = len(lines[0])

	for i, line := range lines {
		maze.Grid = append(maze.Grid, make([]int, maze.ColCount))

		for j, char := range line {
			maze.Grid[i][j], err = strconv.Atoi(string(char))
			check(err)

			if maze.Grid[i][j] == EntryVal {
				maze.Entry[0] = i
				maze.Entry[1] = j

			} else if maze.Grid[i][j] == EndVal {
				maze.End[0] = i
				maze.End[1] = j
			}
		}
	}

	return nil
}

func (maze Maze) canGoUpFrom(lineIndex, colIndex int) bool {
	return lineIndex-1 >= 0 && maze.Grid[lineIndex-1][colIndex] != WALL
}

func (maze Maze) canGoDownFrom(lineIndex, colIndex int) bool {
	return lineIndex+1 < maze.LineCount && maze.Grid[lineIndex+1][colIndex] != WALL
}

func (maze Maze) canGoLeftFrom(lineIndex, colIndex int) bool {
	return colIndex-1 >= 0 && maze.Grid[lineIndex][colIndex-1] != WALL
}
func (maze Maze) canGoRightFrom(lineIndex, colIndex int) bool {
	return colIndex+1 < maze.ColCount && maze.Grid[lineIndex][colIndex+1] != WALL
}
func check(err error) {
	if err != nil {
		panic(err)
	}
}

func trackContains(track [][]int, point []int) bool {
	for _, p := range track {
		if p[0] == point[0] && p[1] == point[1] {
			return true
		}
	}
	return false
}
