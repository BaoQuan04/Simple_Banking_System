CREATE TABLE users(
    uname VARCHAR(50) UNIQUE NOT NULL,
    urole VARCHAR(50) NOT NULL
);

CREATE TABLE userwithwallet(
    user_id BIGSERIAL NOT NULL REFERENCES users(id),
    uname VARCHAR(50) NOT NULL REFERENCES users(username),
    upassword VARCHAR(50) DEFAULT '1',
    ubalance INT DEFAULT 5000
);

CREATE TABLE totalpoints(
	total BIGINT DEFAULT 50000,
	pointin BIGINT;
	pointout BIGINT;
);

CREATE TABLE history(
    user_id BIGSERIAL NOT NULL REFERENCES users(id),
    from_user VARCHAR(50) NOT NULL,
    to_user VARCHAR(50) NOT NULL,
    points_transferred INT NOT NULL,
    ubalance INT NOT NULL,
    date_excute timestamp NOT NULL DEFAULT NOW() 
);

SELECT urole FROM users WHERE uname = '' /*check uname exist in SQL*/
SELECT * FROM userwithwallet LIMIT 5 ORDER BY ASC; /*print 5 roll from beginning of table*/ 


INSERT INTO users(uname, urole) values ('', ''); /*add user or admin to control in system */
INSERT INTO userwithwallet(uname, upassword, upoints) values ();/* add user info */
INSERT INTO history(user_id, from_user, to_user, points_transferred, ubalance) values(); /*add history for tracking*/
UPDATE userwithwallet SET ubalance = ubalance - history.points_transeferred WHERE userwithwallet.user_id = history.user_id; /*update sender's balance*/
UPDATE userwithwallet SET ubalance = ubalance + history.points_transeferred; /* update receiver's balance */


BEGIN
	BEGIN
	