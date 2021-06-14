import os

def content_from_file(file_path):
    f_template = open(file_path, 'r')
    content = f_template.readlines()
    f_template.close()
    return content

def write_content_to_file(content, file_path):
    f = open(file_path, 'w')
    f.write(content)
    f.close()

def list_files():
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
    return h_file_set, cpp_file_set

h_list_file, cpp_list_file = list_files()
cmakelist = content_from_file("../CMakeLists.txt")

h_writed = False
cmakelist_h = []
for line in cmakelist:
    if not (".h" in line and 'Classes' in line):
        cmakelist_h.append(line)

    if ".h" in line and 'Classes' in line and not h_writed:
        h_writed = True
        for l in h_list_file:
            cmakelist_h.append(l + '\n')


cpp_writed = False
cmakelist_h_cpp = []
for line in cmakelist_h:
    if not (".cpp" in line and 'Classes' in line):
        cmakelist_h_cpp.append(line)

    if ".cpp" in line and 'Classes' in line and not cpp_writed:
        cpp_writed = True
        for l in cpp_list_file:
            cmakelist_h_cpp.append(l + '\n')

with open("../CMakeLists.txt", 'w') as f:
    f.writelines(cmakelist_h_cpp)
    f.close()