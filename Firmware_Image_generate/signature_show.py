from cgitb import text
from cmath import nan
from pickle import NONE
import struct
import sys
from neowine_sig import *
from encryption import *
from PyQt5.QtWidgets import *
from PyQt5.QtGui import *
from PyQt5.QtCore import *
from PyQt5 import uic

class MyApp(QDialog):
    n_1 = 0
    e_1 = 0
    d_1 = 0
    n_2 = 0
    e_2 = 0
    data = 0
    
    def dialog_open(self,input):
        if input:
            fname = input
        else:        
            fname = QFileDialog.getOpenFileName(self)  
        if fname[0]:
            self.pathLabel.setText(fname[0])
            print('filepath : ', fname[0])
            print('filesort : ', fname[1])
            
            # MyApp.data = fname[0]
            
            with open(fname[0], "rb") as f:
                MyApp.data = f.read()
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
            
            MyApp.n_1 = key.n
            MyApp.e_1 = key.e
            MyApp.d_1 = key.d
            
        else:
            QMessageBox.about(self, 'warning', '파일을 선택하지 않았습니다')
    def encrypt(self,ver):
        print(ver)
        sig = encryption(MyApp.data,key_gen(MyApp.n_1,MyApp.e_1,MyApp.d_1),MyApp.n_2,MyApp.e_2,ver,1)
        return sig
        
    def second_window(self,data):
        window_2 = second(data)
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
    def __init__(self,data):
        super().__init__()
        layout = QVBoxLayout()
        self.textEdit = QTextEdit(self)
        layout.addWidget(self.textEdit)
        self.setLayout(layout)
        self.setWindowTitle('data_shown')
        self.setWindowIcon(QIcon('drone.jpg'))
        self.get_data(data)
        self.resize(600,700)
        self._initUi_()

    def _initUi_(self):
        self.show()

    def get_data(self,data):
        self.textEdit.setText(str(data))

if __name__ == '__main__':
   app = QApplication(sys.argv)
   ex = MyApp()
   sys.exit(app.exec_())
