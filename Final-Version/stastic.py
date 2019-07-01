import os

# lines = 0

# files = os.walk('.')
# for now_dir in files:
#     for file in now_dir[2]:
#         if file.endswith('.cpp') or file.endswith('.h'):
#             with open(file, "r", encoding='utf-8') as f:
#                 try:
#                     for temp in f.readlines():
#                         lines = lines+1
#                 except Exception:
#                     pass
                

# print('total line: ')
# print(lines)

file = 'config'

lines = 0

# with open(file+'.cpp',  encoding='utf-8') as f:
#     for line in f.readlines():
#         if line != '\n':
#             lines += 1

if os.path.isfile(file+'.h'):
    with open(file+'.h',  encoding='utf-8') as f:
        for line in f.readlines():
            if line != '\n':
                lines += 1

print(str(lines))