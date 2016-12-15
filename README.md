# Brainfuck
A collection of brainfuck interpreters written in C, Ruby and Python

I have tried to keep the basic algorithm same in all three languages which I'll describe here - 

1. The filename has to be supplied as an argument. We open the file in the program, and pass it to the function <b>filter_code</b>
      This function filters the code and remove any character that's not one of the 8 instructions.
      
2. We take the clean code and pass it to the function <b>loopmap</b>.
      This function creates a map of [ and ]'s. (We talk about it later).
      
3. We take the clean code and pass it to a function called <b>evaluate</b>. This function reads the code and evaluates it.

#Important variable names:

1. memcells - The memory cells. At the start of the program we initialize it as 30000 zeroes.
2. memptr - The memory pointer. This variable points to the memory cells.
3. map_loops - The map of the [ and ]'s

#How we handle instructions:

1. <b>></b>  We increase memptr by 1 so that it points to the next cell. If it tries to point outside memory cells, we exit showing an error

2. <b><</b>  We decrease memptr by 1 so that it points to the previous cell. If it tries to point before starting of memory cells, we exit showing an error

3. <b>+</b> We increase the current pointed value by 1. If it gets more than 255, we wrap it up.

4. <b>-</b> We decrease the current pointed value by 1. if it gets less than 0, we wrap it up.

5. <b>.</b> We print the character of current pointed cell.

6. <b>,</b> We take one character input and put its ASCII value in current pointed cell.

Handling [ and ] is a bit tricky. The rule is <i>" When we encounter a [ we check if the current pointed value is 0 or not. If not, then go on normally. If 0 then jump to the corresponding balancing ]. And when we encounter a ], we go back to its corresponding ["</i>
To achieve this, inside <b>loopmap</b> we create a map of the indices of [ and ]'s
First we create a stck. If we encounter a [, we push its index into the stack. When we encounter a ], we pop the stack. this popped value must be its balancing [. So, we map the popped value to the index of ].
Also, if we encounter a ], but the stack is empty, it means that there are more ] than [. We exit showing an error.
And after mapping, if the stack is still empty, it means that there are more [ than ]. we exit showing an error.
