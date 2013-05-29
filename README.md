Fast Solver of Closely Related Quadratic Programming Problems
======
Goodtech and MathConsult have developed a tool called PROMAPS.
It calculates the power delivery as a function of demand, and the probability
for undelivered energy for each load branch in the system, and in the 
system as a whole. This problem is formulated as several closely related 
quadratic programming (QP) problems, whose only difference is in the column
bounds. A major part of the bottleneck of PROMAPS, has been found to be
calls to a QP solver. The aim of this thesis is to develop a fast solver
with the characteristics of these QP problems. The characteristics of the 
QP problems suggest that methods based upon linear programming (LP) are 
suited for solving them. In this thesis, we present an iterative QP solver
using methods based upon linear programming. We also present a tree structured
method that reduces the number of calls to a QP solver by a significant amount.
