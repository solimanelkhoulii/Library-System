-- insert 5 patrons
INSERT INTO USERS(username, usertype, loan_count) VALUES ('Patron1', 'Patron', 0);
INSERT INTO USERS(username, usertype, loan_count) VALUES ('Patron2', 'Patron', 0);
INSERT INTO USERS(username, usertype, loan_count) VALUES ('Patron3', 'Patron', 0);
INSERT INTO USERS(username, usertype, loan_count) VALUES ('Patron4', 'Patron', 0);
INSERT INTO USERS(username, usertype, loan_count) VALUES ('Patron5', 'Patron', 0);

-- insert 1 librarian
INSERT INTO USERS(username, usertype, loan_count) VALUES ('Librarian', 'Librarian', 0);

-- insert 1 administrator
INSERT INTO USERS(username, usertype, loan_count) VALUES ('Administrator', 'Administrator', 0);

-- 5 fiction books
INSERT INTO CATALOGUE(item_id, item_type, title, author, is_available, queue_position) VALUES
(1, 'Fiction', 'The Great Gatsby', 'F. Scott Fitzgerald', 1, 2),
(2, 'Fiction', '1984', 'George Orwell', 1, 1),
(3, 'Fiction', 'Pride and Prejudice', 'Jane Austen', 1, 3),
(4, 'Fiction', 'To Kill a Mockingbird', 'Harper Lee', 1, 1),
(5, 'Fiction', 'The Catcher in the Rye', 'J.D. Salinger', 1, 2);

-- 5 non-fiction books with Dewey Decimal
INSERT INTO CATALOGUE(item_id, item_type, title, author, dewey_decimal, is_available, queue_position) VALUES
(6, 'Non-Fiction', 'Sapiens: A Brief History of Humankind', 'Yuval Noah Harari', '909.82', 1, 1),
(7, 'Non-Fiction', 'Cosmos', 'Carl Sagan', '520', 1, 2),
(8, 'Non-Fiction', 'The Selfish Gene', 'Richard Dawkins', '576.5', 1, 1),
(9, 'Non-Fiction', 'A Brief History of Time', 'Stephen Hawking', '523.1', 1, 3),
(10, 'Non-Fiction', 'Quiet: The Power of Introverts', 'Susan Cain', '155.232', 1, 1);

-- 3 magazines with issue numbers and publication dates
INSERT INTO CATALOGUE(item_id, item_type, title, author, issue_number, publication_date, is_available, queue_position) VALUES
(11, 'Magazine', 'National Geographic', 'Various', '12', '2025-10-01', 1, 1),
(12, 'Magazine', 'The Economist', 'Various', '45', '2025-11-05', 1, 2),
(13, 'Magazine', 'Wired', 'Various', '9', '2025-09-15', 1, 1);

-- 3 movies with genre and rating
INSERT INTO CATALOGUE(item_id, item_type, title, author, genre, rating, is_available, queue_position) VALUES
(14, 'Movie', 'Inception', 'Christopher Nolan', 'Sci-Fi', 13, 1, 1),
(15, 'Movie', 'Parasite', 'Bong Joon-ho', 'Thriller', 18, 1, 2),
(16, 'Movie', 'Toy Story', 'John Lasseter', 'Animation', 0, 1, 1);

-- 4 video games with genre and rating
INSERT INTO CATALOGUE(item_id, item_type, title, author, genre, rating, is_available, queue_position) VALUES
(17, 'VideoGame', 'The Legend of Zelda: Breath of the Wild', 'Nintendo', 'Action-Adventure', 10, 1, 1),
(18, 'VideoGame', 'Red Dead Redemption 2', 'Rockstar Games', 'Action-Adventure', 17, 1, 2),
(19, 'VideoGame', 'Stardew Valley', 'ConcernedApe', 'Simulation', 0, 1, 1),
(20, 'VideoGame', 'Overwatch 2', 'Blizzard Entertainment', 'First-Person Shooter', 13, 1, 1);
