class Matrix:
    def __init__(self, r: int, c: int):
        self.r = r
        self.c = c
        self._array = [None] * r * c

    def __getitem__(self, pos):
        if isinstance(pos, int):
            return self._array[pos * self.c : (pos + 1) * self.c]
        elif len(pos) == 2:
            i, j = pos
            return self._array[i * self.c + j]
        else:
            raise IndexError

    def __setitem__(self, pos, value):
        if len(pos) == 1 and len(value) == self.c:
            for j, val in enumerate(value):
                self._array[pos * self.c + j] = val
        elif len(pos) == 2:
            i, j = pos
            self._array[i * self.c + j] = value
        else:
            raise IndexError
        i, j = pos
        self._array[i * self.c + j] = value

    def __str__(self):
        return str(self._array)


m = Matrix(5, 2)
y = m[:2, 1]
print(m)
print(y)
