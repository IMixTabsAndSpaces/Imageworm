
import mysql.connector
from mysql.connector import errorcode
from datetime import datetime
import os
lib = os.environ['IW_LIB']
path = os.path.join(lib,'tools')+'FSConvert'
from helpers.FSConvert import xmldata

##===============================================
class DatabaseUtility: 
	def __init__(self, database, tableName):
		self.db = database
		self.tableName = tableName
		self.cnx = mysql.connector.connect(
            host="localhost",
            user="root",
            passwd="Qq1!Ww2@",
            )
		self.cursor = self.cnx.cursor()

		self.ConnectToDatabase()
		self.CreateTable()
		
	def ConnectToDatabase(self):
		try:
			self.cnx.database = self.db
		except mysql.connector.Error as err:
			if err.errno == errorcode.ER_BAD_DB_ERROR:
				self.CreateDatabase()
				self.cnx.database = self.db
			else:
				print(err.msg)

	def CreateDatabase(self):
		try:
			self.RunCommand("CREATE DATABASE %s DEFAULT CHARACTER SET 'utf8';" %self.db)
		except mysql.connector.Error as err:
			print("Failed creating database: {}".format(err))

	def CreateTable(self):
		cmd = (" CREATE TABLE IF NOT EXISTS " + self.tableName + " ("
			" Worm_ID INT UNSIGNED NOT NULL AUTO_INCREMENT PRIMARY KEY,"
			" series VARCHAR(30) NOT NULL DEFAULT 'SeriesName',"
			" date TIMESTAMP,"
			" person char(50) NULL,"
			" strain VARCHAR(60) NULL,"
			" treatments VARCHAR(60) NULL,"
			" redsig VARCHAR(60) NULL,"
			" imageloc VARCHAR(200) NULL,"
			" timepts INT NOT NULL DEFAULT 60,"
			" annots VARCHAR(200) NULL,"
			" acetree VARCHAR(200) NULL,"
			" edited_by VARCHAR(60) NULL,"
			" edited_time_pts VARCHAR(60) NULL,"
			" edited_cells VARCHAR(60) NULL,"
			" checked_by CHAR(50) NULL,"
			" comments VARCHAR(60) NULL,"
			" status CHAR(10) NOT NULL DEFAULT 'NEW'"
			") ENGINE=InnoDB;")	
		self.RunCommand(cmd)

	def GetTable(self):
		self.CreateTable()
		return self.RunCommand("SELECT * FROM %s;" % self.tableName)

	def GetColumns(self):
		return self.RunCommand("SHOW COLUMNS FROM %s;" % self.tableName)
	
	def GetRow(self, ID):
		return self.RunCommand("SELECT * FROM {} WHERE Worm_ID = {};".format(self.tableName, ID))


	def RunCommand(self, cmd):
		print ("RUNNING COMMAND: " + cmd)
		try:
			self.cursor.execute(cmd)
		except mysql.connector.Error as err:
			print ('ERROR MESSAGE: ' + str(err.msg))
			print ('WITH ' + cmd)
		try:
			msg = self.cursor.fetchall()
		except:
			msg = self.cursor.fetchone()
		return msg

	def AddEntryToTable(self):
		cmd = " INSERT INTO " + self.tableName
		cmd += " VALUES ();"
		self.RunCommand(cmd)
	
	def AddXmlToTable(self, files):
		for f in files:
			name = os.path.basename(f)
			dat = xmldata(os.path.join(f, 'dats', name+'.xml'))
			self.RunCommand(dat.mysqlentry('worms'))

	def delEntry(self, ID):
		cmd = "DELETE FROM " + self.tableName + " WHERE Worm_ID=" + ID + ";"
		self.RunCommand(cmd)
	
	def editTableEntry(self, valueDic={}):
		colName = []
		values = []
		for key in valueDic:
			if not valueDic[key] == "":
				colName.append(key)
				values.append("'"+
				  valueDic[key]+"'")
		
		colName = "("+", ".join(colName)+")"
		values = "("+", ".join(values)+")"
		cmd = " REPLACE INTO " + self.tableName + colName
		cmd += " VALUES"+ values +";"
		self.RunCommand(cmd)
	

	def __del__(self):
		self.cnx.commit()
		#self.cursor.close()
		self.cnx.close()

##===============================================
##===============================================


if __name__ == '__main__':
	db = 'test2'
	tableName = 'student1'

	dbu = DatabaseUtility(db, tableName)

	# dbu.AddEntryToTable ('testing')
	# dbu.AddEntryToTable ('testing2')
	# print (dbu.GetColumns())
	# print (dbu.GetTable())