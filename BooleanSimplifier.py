from sympy import symbols, And, Not, Or, simplify

# 定义布尔变量
j1, j2, j3, zr, ng = symbols('j1 j2 j3 zr ng')

# 定义布尔表达式
expr = Or(
    And(Not(j1), Not(j2), j3, Not(zr), Not(ng)),
    And(Not(j1), j2, Not(j3), zr),
    And(Not(j1), j2, j3, Not(ng)),
    And(j1, Not(j2), Not(j3), ng),
    And(j1, Not(j2), j3, Not(zr)),
    And(j1, j2, Not(j3), ng),
    And(j1, j2, Not(j3), zr),
    And(j1, j2, j3)
)

# 化简表达式
simplified_expr = simplify(expr)
print(simplified_expr)
