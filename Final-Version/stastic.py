import os

lines = 0

files = os.walk('.')
for now_dir in files:
    for file in now_dir[2]:
        if file.endswith('.cpp') or file.endswith('.h'):
            print(file, end='')
            cost = 0
            with open(file, "r", encoding='utf-8') as f:
                try:
                    for temp in f.readlines():
                        if temp != "\n":
                            cost = cost+1
                except Exception:
                    print('fuck')
                    pass
            print(": "+str(cost))
            lines += cost
                

print('total line: ')
print(lines)

file = 'optimizer'

lines = 0

if os.path.isfile(file+'.cpp'):
    with open(file+'.cpp',  encoding='utf-8') as f:
        for line in f.readlines():
            if line != '\n':
                lines += 1

if os.path.isfile(file+'.h'):
    with open(file+'.h',  encoding='utf-8') as f:
        for line in f.readlines():
            if line != '\n':
                lines += 1

print(str(lines))