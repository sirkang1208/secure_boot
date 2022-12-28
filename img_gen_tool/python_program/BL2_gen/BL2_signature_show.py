import sys
import os.path
from subprocess import *
from cgitb import text
from cmath import nan
from pickle import NONE
from BL2_neowine_sig import *
from BL2_encryption import *
from PyQt5.QtWidgets import *
from PyQt5.QtGui import *
from PyQt5.QtCore import *
from PyQt5 import uic

class MyApp(QDialog):
    str1 = " "
    str2 = " "
    str3 = " "
    
    def dialog_open(self,input):
        if input:
            fname = input
        else:        
            fname = QFileDialog.getOpenFileName(self)  
        if fname[0]:
            self.pathLabel.setText(fname[0])
            print('filepath : ', fname[0])
            print('filesort : ', fname[1])
            MyApp.str1 = fname[0].split('/');
        else:
            QMessageBox.about(self, 'warning', '파일을 선택하지 않았습니다')
    
    def get_key(self,input):
        if input:
            fname = input
        else:        
            fname = QFileDialog.getOpenFileName(self)   
        if fname[0]:
            self.pathLabel.setText(fname[0])
            print('filepath : ', fname[0])
            print('filesort : ', fname[1])
            
            f_key = open(fname[0], "r")
            key = RSA.importKey(f_key.read())
            
            if key.has_private():
                MyApp.str2 = fname[0].split('/');
            else :
                MyApp.str3 = fname[0].split('/');
        else:
            QMessageBox.about(self, 'warning', '파일을 선택하지 않았습니다')
            
    def encrypt(self,ver):
        command = "bootloader.exe "
        command = command + "./" + MyApp.str1[-1] +" ./" +  MyApp.str2[-1] +" ./" +  MyApp.str3[-1] + " " + ver
        try:
            run(command, shell=True)
            #p = Popen(command, shell=True, stdout=PIPE, stderr=STDOUT, stdin=PIPE)
            #print(p)
        except Exception as e:
            print('예외', e)
        
    def second_window(self):
        window_2 = second()
        window_2.exec_()

    def __init__(self):
        super().__init__()
        QToolTip.setFont(QFont('SansSerif', 10))
        self.btn1 = QPushButton('open file', self)
        self.btn1.clicked.connect(self.dialog_open)
        self.pathLabel = QLabel(self)
        layout = QVBoxLayout()
        layout.addWidget(self.btn1)
        layout.addWidget(self.pathLabel)
        self.setLayout(layout)
        self.setWindowTitle('px4_extracted')
        self.setWindowIcon(QIcon('drone.jpg'))
        self.resize(300,150)
        self.initUI()

    def initUI(self):
        self.show()

class second(QDialog):
    def __init__(self):
        super().__init__()
        layout = QVBoxLayout()
        self.textEdit = QTextEdit(self)
        layout.addWidget(self.textEdit)
        self.setLayout(layout)
        self.setWindowTitle('process result')
        self.setWindowIcon(QIcon('drone.jpg'))
        self.get_data()
        self.resize(300,150)
        self._initUi_()

    def _initUi_(self):
        self.show()

    def get_data(self):
        if(os.path.isfile("BL2_final_image.bin")):
            self.textEdit.setText("success")
        else:
            self.textEdit.setText("Failed")

if __name__ == '__main__':
   app = QApplication(sys.argv)
   ex = MyApp()
   sys.exit(app.exec_())
