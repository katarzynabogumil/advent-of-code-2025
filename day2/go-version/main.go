package main

import (
	"bufio"
	"fmt"
	"os"
	"strconv"
	"strings"
	"time"
)

type RangePair [2]int64

func validateIDP2(id int) bool {
	strID := strconv.Itoa(id)

	for i := 1; i <= len(strID)/2; i++ {
		for j := 0; j+2*i <= len(strID); j += i {
			first := strID[j : i+j]
			second := strID[i+j : j+2*i]

			if first != second {
				break
			}

			if j+2*i == len(strID) {
				return false
			}
		}
	}

	return true
}

func part2(input []RangePair) int64 {
	var sum int64 = 0

	for _, r := range input {
		start := int(r[0])
		end := int(r[1])

		for i := start; i <= end; i++ {
			if !validateIDP2(i) {
				sum += int64(i)
			}
		}
	}

	return sum
}

func validateID(id int) bool {
	strID := strconv.Itoa(id)

	if len(strID)%2 != 0 {
		return true
	}

	midpoint := len(strID) / 2
	first := strID[:midpoint]
	second := strID[midpoint:]

	if first == second {
		return false
	}

	return true
}

func part1(input []RangePair) int64 {
	var sum int64 = 0

	for _, r := range input {
		start := int(r[0])
		end := int(r[1])

		for i := start; i <= end; i++ {
			if !validateID(i) {
				sum += int64(i)
			}
		}
	}

	return sum
}

func parseLine(line string) ([]RangePair, error) {
	ranges := strings.Split(line, ",")
	result := []RangePair{}

	for _, rangeStr := range ranges {
		if rangeStr == "" {
			continue
		}

		dashTokens := strings.Split(rangeStr, "-")

		if len(dashTokens) == 2 {
			start, err := strconv.ParseInt(dashTokens[0], 10, 64)
			if err != nil {
				return nil, fmt.Errorf("error converting start '%s' to int64: %w", dashTokens[0], err)
			}

			end, err := strconv.ParseInt(dashTokens[1], 10, 64)
			if err != nil {
				return nil, fmt.Errorf("error converting end '%s' to int64: %w", dashTokens[1], err)
			}

			result = append(result, RangePair{start, end})
		}
	}

	return result, nil
}

func parseFile(filename string) ([]RangePair, error) {
	file, err := os.Open(filename)
	if err != nil {
		return nil, fmt.Errorf("cannot open file: %s: %w", filename, err)
	}
	defer file.Close()

	scanner := bufio.NewScanner(file)

	if scanner.Scan() {
		line := scanner.Text()
		if line != "" {
			return parseLine(line)
		}
	}

	if err := scanner.Err(); err != nil {
		return nil, fmt.Errorf("error reading file: %w", err)
	}

	return []RangePair{}, nil
}

func main() {
	start := time.Now()

	parsedInput, err := parseFile("../input.txt")
	if err != nil {
		fmt.Fprintf(os.Stderr, "Error: %v\n", err)
		os.Exit(1)
	}

	resPart1 := part1(parsedInput)
	timePart1 := time.Since(start)
	fmt.Printf("Part 1: %d in %s\n", resPart1, timePart1)

	resPart2 := part2(parsedInput)
	timePart2 := time.Since(start)
	fmt.Printf("Part 2: %d in %s\n", resPart2, timePart2)
}
