slist = """lambda
print
define
quote
list
if
car
while
progn
macro
lambda*
eq
cons
+
t
mod
eval
cdr
-
*
<
>
/
macro*
atom""".split("\n")

d = dict((i, []) for i in range(16))

def hashof(s):
    o1 = ord(s[0])
    o2 = 0 if len(s) == 1 else ord(s[1])
    return (o1 + o2) % 16
for s in slist:
    d[hashof(s)].append(s)

for k, v in d.items():
    print(k, v)