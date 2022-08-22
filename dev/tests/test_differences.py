import pandas as pd

df_py = pd.read_csv("./dev/matrix_py.csv")
df_c = pd.read_csv("./dev/matrix_c.csv")

# print(df_py)
# print(df_c)

diff = df_py - df_c

print(diff.to_string())
