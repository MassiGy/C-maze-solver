## Notes about the master-golang branch

The idea behind this branch is to try and see how easy it is to convert a 
conccurent C program to a Go program. After this little experiment, I am pleased to know that Go and C are not that diffrent in terms of thinking and
syntax, besides with Go's CSP modal, it is easier to transform a proprely 
working sequential C function to a concurrent Go function.

CSP: concurrent sequential processes


NOTE: the impelementation of the Go version works perfectly well for the 
recursive approach, but it has some issues when it comes to the goroutine
approach. I suspect that my goroutine management using wait groups is not 
well done. (TODO)




