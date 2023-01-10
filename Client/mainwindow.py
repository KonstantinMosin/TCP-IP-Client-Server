from PyQt5.QtWidgets import QMainWindow, QWidget, QGroupBox
from PyQt5.QtWidgets import QGridLayout, QHBoxLayout
from PyQt5.QtWidgets import QLabel, QLineEdit, QPlainTextEdit
from PyQt5.QtWidgets import QPushButton, QRadioButton

class MainWindow(QMainWindow):
    def __init__(self, parent = None):
        super(MainWindow, self).__init__(parent)
        self.initUI()
            
    def initUI(self):
        self.widget = QWidget()
        self.setCentralWidget(self.widget)

        IPText = QLabel("IP")
        IPLineEdit = QLineEdit()

        PortText = QLabel("port")
        PortLineEdit = QLineEdit()

        delayForSendText = QLabel("delay")
        delayForSendLineEdit = QLineEdit()

        messageTypeButtonbox = QGroupBox()
        messageTypeLayout = QHBoxLayout()
        fastResponseButton = QRadioButton("fast response")
        fastResponseButton.setChecked(True)
        slowResponseButton = QRadioButton("slow response")
        messageTypeLayout.addWidget(fastResponseButton)
        messageTypeLayout.addWidget(slowResponseButton)
        messageTypeButtonbox.setLayout(messageTypeLayout)

        self.logger = QPlainTextEdit()
        self.logger.setReadOnly(True)

        sendButton = QPushButton("send")

        grid = QGridLayout(self.centralWidget())
        grid.addWidget(IPText)
        grid.addWidget(IPLineEdit)
        grid.addWidget(PortText)
        grid.addWidget(PortLineEdit)
        grid.addWidget(delayForSendText)
        grid.addWidget(delayForSendLineEdit)
        grid.addWidget(messageTypeButtonbox)
        grid.addWidget(self.logger)
        grid.addWidget(sendButton)
        self.logger.appendPlainText("I love you")
