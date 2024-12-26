```latex
\begin{algorithm}[H]
\caption{Bottom-Up Local Allocator}
\begin{algorithmic}
\For{each operation $i$, in order from $1$ to $N$, where $i$ has the form $op \ v_{ri_1}, v_{ri_2} \Rightarrow v_{ri_3}$}
    \State $r_x \gets \text{Ensure}(v_{ri_1}, \text{class}(v_{ri_1}))$
    \State $r_y \gets \text{Ensure}(v_{ri_2}, \text{class}(v_{ri_2}))$
    \If{$v_{ri_1}$ is not needed after $i$}
        \State $\text{Free}(r_x, \text{class}(r_x))$
    \EndIf
    \If{$v_{ri_2}$ is not needed after $i$}
        \State $\text{Free}(r_y, \text{class}(r_y))$
    \EndIf
    \State $r_z \gets \text{Allocate}(v_{ri_3}, \text{class}(v_{ri_3}))$
    \State Rewrite $i$ as $op_i \ r_x, r_y \Rightarrow r_z$
    \If{$v_{ri_1}$ is needed after $i$}
        \State $\text{class.Next}[r_x] \gets \text{Dist}(v_{ri_1})$
    \EndIf
    \If{$v_{ri_2}$ is needed after $i$}
        \State $\text{class.Next}[r_y] \gets \text{Dist}(v_{ri_2})$
    \EndIf
    \State $\text{class.Next}[r_z] \gets \text{Dist}(v_{ri_3})$
\EndFor

\Function{Ensure}{$vr, class$}
    \If{$vr$ is already in $class$}
        \State $\text{result} \gets vr$'s physical register
    \Else
        \State $\text{result} \gets \text{Allocate}(vr, class)$
        \State Emit code to move $vr$ into $\text{result}$
    \EndIf
    \State \Return $\text{result}$
\EndFunction

\Function{Allocate}{$vr, class$}
    \If{$\text{class.StackTop} \geq 0$}
        \State $i \gets \text{pop}(class)$
    \Else
        \State $i \gets j$ that maximizes $\text{class.Next}[j]$
        \State Store contents of $j$
    \EndIf
    \State $\text{class.Name}[i] \gets vr$
    \State $\text{class.Next}[i] \gets -1$
    \State $\text{class.Free}[i] \gets \text{false}$
    \State \Return $i$
\EndFunction
\end{algorithmic}
\end{algorithm}
