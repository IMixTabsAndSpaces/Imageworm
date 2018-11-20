
import mysql.connector
from mysql.connector import errorcode
from datetime import datetime

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
			" Series VARCHAR(30) NOT NULL,"
			" Date_entered TIMESTAMP,"
			" Person char(50) NULL,"
			" Strain VARCHAR(60) NULL,"
			" Treatments VARCHAR(60) NULL,"
			" Redsig VARCHAR(60) NULL,"
			" Imageloc VARCHAR(200) NULL,"
			" Timepts VARCHAR(60) NOT NULL,"
			" Annots VARCHAR(200) NULL,"
			" Acetree VARCHAR(200) NULL,"
			" Edited_by VARCHAR(60) NULL,"
			" Edited_time_pts VARCHAR(60) NULL,"
			" Edited_cells VARCHAR(60) NULL,"
			" Checked_by CHAR(50) NULL,"
			" Comments VARCHAR(60) NULL,"
			" Status CHAR(10) NOT NULL DEFAULT 'NEW'"
			") ENGINE=InnoDB;")	
		self.RunCommand(cmd)

	def GetTable(self):
		self.CreateTable()
		return self.RunCommand("SELECT * FROM %s;" % self.tableName)

	def GetColumns(self):
		return self.RunCommand("SHOW COLUMNS FROM %s;" % self.tableName)

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

	def AddEntryToTable(self, seriesName, TimePoint):
		cmd = " INSERT INTO " + self.tableName + " (Series, Timepts)"
		cmd += " VALUES ('%s', '%s' );" % (seriesName, TimePoint)
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