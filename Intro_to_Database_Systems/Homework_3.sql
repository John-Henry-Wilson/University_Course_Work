-- 1
CREATE DATABASE IF NOT EXISTS classassignment;
USE classassignment;
-- 2
CREATE TABLE IF NOT EXISTS Project
(
	project_num INT NOT NULL PRIMARY KEY,
	project_code CHAR(4),
	project_title VARCHAR(45),
	first_name VARCHAR(45),
	last_name VARCHAR(45), 
	project_budget DECIMAL(5,2)
);
-- 3
ALTER TABLE Project 
MODIFY project_num INT AUTO_INCREMENT
;
ALTER TABLE Project 
AUTO_INCREMENT = 10
;
-- 4
ALTER TABLE Project
MODIFY project_budget DEC(10,2)
;
-- 5
INSERT INTO Project (project_code, project_title, first_name, last_name, project_budget)
VALUES ('PC01', 'DIA', 'John', 'Smith', 10000.99), ('PC02', 'CHF', 'Tim', 'Cook', 12000.50), ('PC03', 'AST', 'Rhonda', 'Smith', 8000.40)
;
--
SELECT * FROM Project;
-- 6
CREATE TABLE IF NOT EXISTS PayRoll
(
	employee_num INT PRIMARY KEY AUTO_INCREMENT,
	job_id INT NOT NULL,
	job_desc VARCHAR(40),
	emp_pay DECIMAL (10,2)
);
-- 7
-- 7.i
ALTER TABLE PayRoll 
ADD CHECK(emp_pay>10000)
;
-- 7.ii
ALTER TABLE PayRoll
ALTER job_desc SET DEFAULT 'Data Analyst'
;
-- 7.iii
ALTER TABLE PayRoll
ADD COLUMN pay_date DATE
AFTER job_desc
;
-- 8
ALTER TABLE PayRoll
ADD CONSTRAINT FK_project_num_job_id
FOREIGN KEY (job_id) REFERENCES Project (project_num)
;
-- 9
INSERT INTO PayRoll (job_id, pay_date, emp_pay)
VALUES (10, '2022-06-23', 12000.99), (11, '2022-06-23', 14000.99), (12, '2022-06-23', 16000.99)
;
-- 10
UPDATE PayRoll 
SET emp_pay = emp_pay * 1.1
WHERE employee_num = 2
; 
-- 11
CREATE TABLE Project_backup AS
SELECT * 
FROM Project
WHERE last_name = 'Smith'
;
-- 12
CREATE VIEW PayRoll_View AS
SELECT job_id, job_desc, pay_date
FROM PayRoll
WHERE job_id > 10
;
-- 13
CREATE INDEX IX_pay_date ON PayRoll (pay_date);
-- 14
DELETE FROM Project_backup;
-- 15
DELETE FROM Project WHERE project_num = 10;
-- Explanation: You cannot delete this row because it fails a foreign key constaint meaning that row's PK is a FK for another table and you must drop the constraint first before you can delete it.
-- 16
ALTER TABLE PayRoll
DROP CONSTRAINT FK_project_num_job_id
;
DELETE FROM Project WHERE project_num = 10;