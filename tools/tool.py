import Tkinter as tk
import tkFileDialog
import tkMessageBox
import subprocess
import os

def content_from_file(file_path):
    f_template = open(file_path, 'r')
    content = f_template.readlines()
    f_template.close()
    return ''.join([line for line in content])

def write_content_to_file(content, file_path):
    f = open(file_path, 'w')
    f.write(content)
    f.close()

def create_a_new_class(class_name, saved_path):

    if(saved_path[-1] == "/"):
        saved_path = saved_path[0:-1]

    h_file_path = saved_path+"/{name}.h".format(name=class_name)
    cpp_file_path = saved_path+"/{name}.cpp".format(name=class_name)


    # create h file
    h_file_content_format = content_from_file("../Templates/Class/ClassTemplate.h")
    h_file_content = h_file_content_format % class_name
    write_content_to_file(h_file_content, h_file_path)

    # create cpp file
    cpp_file_content_format = content_from_file("../Templates/Class/ClassTemplate.cpp")
    cpp_file_content = cpp_file_content_format % class_name
    write_content_to_file(cpp_file_content, cpp_file_path)

class CMakelistHelper:
    def __init__(self):
        self.cmakelist_path = "../CMakeLists.txt"
        self.h_list_file, self.cpp_list_file = self.list_files()
        self.cmakelist = self.content_from_file(self.cmakelist_path)
        self.cmakelist_h = []
        self.cmakelist_h_cpp = []

    def content_from_file(self, file_path):
        f_template = open(file_path, 'r')
        content = f_template.readlines()
        f_template.close()
        return content

    def write_content_to_CMakeList_file(self, content):
        with open(self.cmakelist_path, 'w') as f:
            f.writelines(content)
            f.close()

    def list_files(self):
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

    def append_h_files_to_cmakelist(self):
        h_writed = False
        for line in self.cmakelist:
            if not (".h" in line and 'Classes' in line):
                self.cmakelist_h.append(line)

            if ".h" in line and 'Classes' in line and not h_writed:
                h_writed = True
                for l in self.h_list_file:
                    self.cmakelist_h.append(l + '\n')

    def append_cpp_files_to_cmakelist(self):
        cpp_writed = False
        for line in self.cmakelist_h:
            if not (".cpp" in line and 'Classes' in line):
                self.cmakelist_h_cpp.append(line)

            if ".cpp" in line and 'Classes' in line and not cpp_writed:
                cpp_writed = True
                for l in self.cpp_list_file:
                    self.cmakelist_h_cpp.append(l + '\n')


window = tk.Tk(className="Duy's cocos2d-x tools")
window.rowconfigure(0, weight=1)
window.rowconfigure(1, weight=1)
window.rowconfigure(2, weight=1)
window.rowconfigure(3, weight=1)
window.rowconfigure(4, weight=1)
window.columnconfigure(0, weight=1)
window.columnconfigure(1, weight=1)
window.resizable(0, 0)
window.geometry("1000x200+300+300")


# Create classname

lbl_classname = tk.Label(master=window, text="Class name")
lbl_classname.grid(row=0, column=0)

entry_classname = tk.Entry(master=window, width=100)
entry_classname.grid(row=1,column=0)

# Create saved path

lbl_saved_path = tk.Label(master=window, text="Stored Path")
lbl_saved_path.grid(row=2, column=0)

entry_saved_path = tk.Entry(master=window, width=100)
entry_saved_path.grid(row=3,column=0)

def on_entry_saved_path_click(event):
    saved_path = tkFileDialog.askdirectory()
    entry_saved_path.insert(0, saved_path)
    pass

entry_saved_path.bind("<1>", on_entry_saved_path_click)


# Create Add button
def on_btn_add_click():
    classname = entry_classname.get()
    if classname == "":
        tkMessageBox.showerror("Error", "Class name must not empty")
        return

    savedpath = entry_saved_path.get()
    if savedpath == "":
        tkMessageBox.showerror("Error", "Saved path must not empty")
        return
    
    create_a_new_class(classname, savedpath)
    tkMessageBox.showinfo("Info", "Creating {} class success".format(classname))


    
btn_add = tk.Button(master=window, text="Add", command=on_btn_add_click)
btn_add.grid(row=0, column=1, sticky="nsew")

# Create Build CMakelist button
def on_btn_build_click():
    cmakelist_helper = CMakelistHelper()
    cmakelist_helper.append_h_files_to_cmakelist()
    cmakelist_helper.append_cpp_files_to_cmakelist()
    cmakelist_helper.write_content_to_CMakeList_file(cmakelist_helper.cmakelist_h_cpp)

    os.system('cd ../proj.win32 && cmake ..')

btn_build = tk.Button(master=window, text="Build CMakelist", command=on_btn_build_click)
btn_build.grid(row=2, column=1, sticky="nsew")

btn_open_vs2019_project = tk.Button(master=window, text="Open VS2019 Project", command=lambda : os.system('start ../proj.win32/MyRollBall.sln'))
btn_open_vs2019_project.grid(row=4, column=1, sticky="nsew")

window.mainloop()
