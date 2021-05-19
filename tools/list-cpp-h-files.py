import os
root_dir = "../"
classes_dir = "Classes/"

cpp_file_set = []
h_file_set = []

# list file
for dir_, _, files in os.walk(root_dir + classes_dir):
    for file_name in files:
        rel_dir = os.path.relpath(dir_, root_dir)
        rel_file = os.path.join(rel_dir, file_name)
        rel_file = rel_file.replace("\\", "/")
        if file_name[-1] == 'h':
            h_file_set.append(rel_file)
        else:
            cpp_file_set.append(rel_file)


# write file

f = open("file.txt","w")

for line in cpp_file_set:
    f.write(line + "\n")

f.write("\n")

for line in h_file_set:
    f.write(line + "\n")