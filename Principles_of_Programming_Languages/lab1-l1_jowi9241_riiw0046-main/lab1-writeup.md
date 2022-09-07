# Lab 1 Write Up

1. complete

2. 
  - **a.** The use of `pi` at line 4 is bound at line 3 because that variable is in the local scope of the `circumference` function which is resides. The use of `pi` at line 7 is bound at line 1 since the `area` function does not have a variable of the same name declared in its local scope
  - **b.** The use of `x` at line 3 is bound line 2 because `x` is a parameter to the `f` function. The use of `x` at line 6 is bound at line 5 because the x on line 6 is bound within the scope of the case statement on line 5. The use of `x` at line 10 is bound at line 5 because the x on line 10 is bound within the scope of the case statement on line 5. The use of `x` at line 13 is bound line 1 because it is being used as an argument in the function `f(x)`.

3. Yes, because the body expression of `g` is well-typed with type ((Int, Int), Int).
`g`: ((Int, Int), Int) because
      b: (Int, Int)
      a: Int