-- in case you want to alter fields in the tables and db alerady exists
-- we can drop tables first
PRAGMA foreign_keys = OFF;
DROP TABLE IF EXISTS CATALOGUE;
DROP TABLE IF EXISTS USERS;
DROP TABLE IF EXISTS LOANS;
DROP TABLE IF EXISTS HOLDS;
PRAGMA foreign_keys = ON; -- avoids drop errors when dependencies between tables

-- catalogue items contain all items of all types
-- unused fields in specific item types can be null
CREATE TABLE IF NOT EXISTS CATALOGUE(
    item_id integer primary key,
    item_type text not null,
    title text not null,
    author text not null,
    dewey_decimal text,
    isbn text, 
    publication_date text,
    issue_number text,
    genre text,
    rating integer,
    format text,
    total_copies int not null default 1,
    is_available boolean not null default 1,
    queue_position integer not null default 0
);

-- could extrapolate user types into own tables if desirable
CREATE TABLE IF NOT EXISTS USERS(
    username text primary key,
    usertype text not null,
    loan_count integer not null default 0
);

-- users can borrow the same item multiple times
-- so we use loan_id as pk instead of composite with item_id and username
CREATE TABLE IF NOT EXISTS LOANS(
    loan_id integer primary key, 
    username text not null,
    item_id integer not null,
    borrow_date text not null,
    due_date text not null,
    foreign key (username) references users(username),
    foreign key (item_id) references catalogue(item_id)
);

-- username and item_id as a composite primary key
CREATE TABLE IF NOT EXISTS HOLDS(
    item_id integer not null,
    username text not null,
    queue_position integer not null,
    foreign key (username) references users(username),
    foreign key (item_id) references catalogue(item_id)
    primary key (item_id, username)
);