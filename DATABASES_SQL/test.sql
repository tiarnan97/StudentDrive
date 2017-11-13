-- Clean up from any previous tutorial actions.
DROP TABLE Transaction;
DROP TABLE Book;
DROP TABLE Borrower;
DROP TABLE Author;
DROP TABLE TransactType;



 
-- Create the tables needed.

CREATE TABLE TransactType(
	TransactType number PRIMARY KEY,
	TransactDesc varchar2(50)
);

CREATE TABLE Author(
	AuthorID Number(4) PRIMARY KEY,
	AuthorFirstName varchar2(50),
	AuthorLastName varchar2(50)
);

CREATE TABLE Book (
   Bookid NUMBER(4) PRIMARY KEY,
   Title VARCHAR2(50),
   AuthorID Number(4),
   CONSTRAINT book_author_fk FOREIGN KEY (AuthorID)
      REFERENCES Author(Authorid)
   );
 
CREATE TABLE Borrower (
   Borrowerid NUMBER(4)  PRIMARY KEY,
   Last_name VARCHAR2(30),
   First_name VARCHAR2(30),
   Street_address VARCHAR2(50),
   Zip VARCHAR2(50)
   );
 
CREATE TABLE Transaction (
   Transactionid NUMBER PRIMARY KEY,
   Borrowerid NUMBER (4),
   Bookid NUMBER (4),
   Transaction_date DATE,
   Transaction_type Number(1),
   CONSTRAINT trans_book_fk FOREIGN KEY (BookID)
      REFERENCES Book(BookId),
CONSTRAINT trans_borrow_fk FOREIGN KEY (BorrowerID)
      REFERENCES Borrower(BorrowerId),
CONSTRAINT trans_type_fk FOREIGN KEY (Transaction_type)
      REFERENCES TransactType(TransactType)
   );


-- Insert and query data.
INSERT INTO TRANSACTTYPE (TRANSACTTYPE, TRANSACTDESC)  VALUES (1, 'Borrow');
INSERT INTO TRANSACTTYPE (TRANSACTTYPE, TRANSACTDESC)  VALUES (2, 'Return');
INSERT INTO TRANSACTTYPE (TRANSACTTYPE, TRANSACTDESC) VALUES (3, 'Fine');

INSERT INTO AUTHOR (AuthorID, AuthorFirstName, AuthorLastName) VALUES (1,'Melville', 'Herman');
INSERT INTO AUTHOR (AuthorID, AuthorFirstName, AuthorLastName)  VALUES (2,'Scammer', 'Ima');
INSERT INTO AUTHOR (AuthorID, AuthorFirstName, AuthorLastName) VALUES (3,'Blissford', 'Serenity');
INSERT INTO AUTHOR (AuthorID, AuthorFirstName, AuthorLastName) VALUES (4,'Whodunit', 'Rodney');
INSERT INTO AUTHOR (AuthorID, AuthorFirstName, AuthorLastName) VALUES (5,'Abugov', 'D.');

 
INSERT INTO Book (BookId, Title, AuthorID) VALUES (100, 'Moby Dick', 1);
INSERT INTO Book (BookId, Title, AuthorID) VALUES (101, 'Get Rich Really Fast',  2);
INSERT INTO Book (BookId, Title, AuthorID) VALUES (102, 'Finding Inner Peace',  3);
INSERT INTO Book (BookId, Title, AuthorID) VALUES (103, 'Great Mystery Stories',  4);
INSERT INTO Book (BookId, Title, AuthorID) VALUES (104, 'Software Wizardry',  5);
 
INSERT INTO Borrower (BorrowerID, Last_name, First_name ,Street_address,Zip ) VALUES  (100, 
   'Smith', 'Jane', '123 Main Street', 'Mytown, MA 01234');
INSERT INTO Borrower (BorrowerID, Last_name, First_name ,Street_address,Zip ) VALUES  (101,
   'Chen', 'William', '16 S. Maple Road', 'Mytown, MA 01234');
INSERT INTO Borrower (BorrowerID, Last_name, First_name ,Street_address,Zip ) VALUES  (102, 
   'Fernandez', 'Maria', '502 Harrison Blvd.', 'Sometown, NH 03078');
INSERT INTO Borrower (BorrowerID, Last_name, First_name ,Street_address,Zip ) VALUES  (103, 
   'Murphy', 'Sam', '57 Main Street', 'Mytown, MA 01234');
 
-- Sysdate is a system function to return the current date
INSERT INTO Transaction (TransactionID, BorrowerID,  BookID, Transaction_date, Transaction_Type)
  VALUES (1,100, 101, SYSDATE, 1);
INSERT INTO Transaction (TransactionID, BorrowerID,  BookID, Transaction_date, Transaction_Type)
  VALUES (2,100, 102, SYSDATE, 2);
INSERT INTO Transaction  (TransactionID, BorrowerID,  BookID, Transaction_date, Transaction_Type)
  VALUES (3,101, 103, SYSDATE, 3);
INSERT INTO Transaction  (TransactionID, BorrowerID,  BookID, Transaction_date, Transaction_Type)
  VALUES (4,101, 102, SYSDATE, 1);
INSERT INTO Transaction  (TransactionID, BorrowerID,  BookID, Transaction_date, Transaction_Type)
  VALUES (5,102, 103, SYSDATE, 1);
INSERT INTO Transaction  (TransactionID, BorrowerID,  BookID, Transaction_date, Transaction_Type)
  VALUES (6,103, 104, SYSDATE, 2);
INSERT INTO Transaction  (TransactionID, BorrowerID,  BookID, Transaction_date, Transaction_Type)
  VALUES (7,100, 104, SYSDATE, 1);
INSERT INTO Transaction  (TransactionID, BorrowerID,  BookID, Transaction_date, Transaction_Type)
  VALUES (8,102, 102, SYSDATE, 2);
INSERT INTO Transaction (TransactionID, BorrowerID,  BookID, Transaction_date, Transaction_Type)
  VALUES (9,102, 104, SYSDATE, 1);
INSERT INTO Transaction  (TransactionID, BorrowerID,  BookID, Transaction_date, Transaction_Type)
  VALUES (10,101, 104, SYSDATE, 1);
  commit;
  
-- Select statements
SELECT  *  FROM  BOOK;
SELECT FIRST_NAME, LAST_NAME, STREET_ADDRESS FROM BORROWER;
INSERT 'Sam', 'Sheppard', '70 Oaks Avenue' INTO BORROWER;
-- projection
describe book;
SELECT  title  FROM  BOOK;
-- restriction
Select title from BOOK where AuthorID BETWEEN 1 AND 3;
-- changing output
select title "Book Title", AuthorID "Author Identifier"  from BOOK where AuthorID BETWEEN 1 AND 3;
