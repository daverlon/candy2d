
f = open("tiles_list.txt", "r")
res = ""

for line in f.readlines():
    res += line.replace(" ", ", ")

print(res)

f = open("l.txt", "w")
f.write(res)