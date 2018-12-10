<img src = "https://github.com/sgcurtis/CS4711/blob/master/Logo.png" width=300 height=300>

Conway's Game of Life is a cellular atomaton that simulates generations through the creation and deletion of cells. 


## Rules
* The cells can either be alive or dead.
* Any living cell with less than two live neighbors dies.
* Any living cell with more than three live neighbors dies.
* Any living cell with two or three live neighbors stays alive for the next generation.
* Any dead cell with exactly three living neighbors becomes a live cell.
* The user determines the first generation and all following generations will be determined through the rules.

## Testing
* [CUnit](http://cunit.sourceforge.net/) - build tests
* [Gcov](https://gcc.gnu.org/onlinedocs/gcc/Gcov.html) - code coverage
* [Lcov](http://ltp.sourceforge.net/coverage/lcov.php) - code coverage

## Built With
* C - Programming Language
* [Gradle](https://gradle.org/) - used to build testing suite

## Versioning
* [GitHub](https://github.com/) - published versions to repo when necessary

## Acknowledgments
* Product Owner: Professor James Walker
* Scrum Master: Sarah Curtis
* Developers: Michael Bandrowski, Dan Schuen, Kyle Smith, Will Stockreef
