-- create table
CREATE TABLE accounts(
    uname VARCHAR(50) UNIQUE NOT NULL,
    urole VARCHAR(50) NOT NULL
);

CREATE TABLE users(
    user_id BIGSERIAL PRIMARY KEY,
    uname VARCHAR(50) UNIQUE NOT NULL,
    upassword VARCHAR(50) DEFAULT '1',
    wallet_id BIGSERIAL UNIQUE,
	balance BIGINT DEFAULT 5000 
);

CREATE TABLE totalpoints(
	total BIGINT DEFAULT 50000,
	pointin BIGINT;
	pointout BIGINT;
);

CREATE TABLE Transactioncode(
	wallet_id BIGINT REFERENCES users(wallet_id),
	transaction_id BIGSERIAL PRIMARY KEY
);

CREATE TABLE history(
    transaction_id BIGINT NOT NULL REFERENCES Transactioncode(transaction_id),
	from_user_wallet BIGINT NOT NULL REFERENCES users(wallet_id),
    to_user_wallet BIGINT NOT NULL REFERENCES users(wallet_id),
    points_used INT NOT NULL,
    balance_before BIGINT NOT NULL,
	balance_after BIGINT NOT NULL,
    date_excute timestamp NOT NULL DEFAULT NOW()::TIMESTAMP(0), -- local time generate when insert into history 
	status VARCHAR(50) NOT NULL
);

-- don't know
SELECT urole FROM users WHERE uname = '' /*check uname exist in SQL when logging*/
SELECT * FROM users LIMIT 5 ORDER BY ASC; /*print 5 roll from beginning of users table*/ 


INSERT INTO users(uname, urole) values ('', ''); /*add user or admin to control in system */
INSERT INTO userwithwallet(uname, upassword, upoints) values ();/* add user info */
INSERT INTO history(user_id, from_user, to_user, points_transferred, ubalance) values(); /*add history for tracking*/
UPDATE userwithwallet SET ubalance = ubalance - history.points_transeferred WHERE userwithwallet.user_id = history.user_id; /*update sender's balance*/
UPDATE userwithwallet SET ubalance = ubalance + history.points_transeferred WHERE userwithwallet.uname = history.to_user; /* update receiver's balance */


-- update pointout for each user created
UPDATE TABLE totalpoints SET pointout := pointout + 5000;


-- check username exist in SQL for logging or transfer
CREATE OR REPLACE FUNCTION check_user(name VARCHAR(50))
RETURNS TEXT AS $$
DECLARE
    wallet_id INT;
BEGIN
    -- query in SQL
    SELECT u.wallet_id INTO wallet_id
    FROM users u
    WHERE u.uname = name;
    
    -- check condition
    IF NOT FOUND THEN
        RETURN format('Tên hoặc mật khẩu không hợp lệ');
	ELSE 
		RETURN ('Chào mừng', wallet_id);
    END IF;

-- catch exceptions
EXCEPTION
        WHEN OTHERS THEN
            -- Catch any errors and return a failure message
            RETURN format('Giao dịch bị hủy do lỗi', SQLERRM);
END;
$$ LANGUAGE plpgsql;




-- transfer and catch exceptions
CREATE OR REPLACE FUNCTION complex_transaction(A BIGINT, B BIGINT, d BIGINT)
RETURNS TEXT AS $$
DECLARE
    balance_A BIGINT;
    balance_B BIGINT;
    balance_A_before BIGINT;
    Tcode_id BIGINT;
	txn_status VARCHAR(50);
BEGIN
    -- Start transaction block
    BEGIN
        -- Get the balances of wallet A and B
        SELECT balance INTO balance_A FROM users WHERE wallet_id = A;
        SELECT balance INTO balance_B FROM users WHERE wallet_id = B;

        -- Always generate a transaction_id regardless of success or failure
        INSERT INTO Transactioncode(wallet_id)
        VALUES (A)
        RETURNING transaction_id INTO Tcode_id;

        -- Check if balance of A is sufficient for the transaction
        IF balance_A >= d THEN
            -- Update the balances for wallet A and B
            UPDATE users SET balance = balance - d WHERE wallet_id = A;
            UPDATE users SET balance = balance + d WHERE wallet_id = B;

            -- Mark the transaction as successful
            txn_status := 'Success';
        ELSE
            -- Mark the transaction as failed due to insufficient balance
            txn_status := 'Failed';
        END IF;

            -- Insert into history table for tracking
          
		INSERT INTO history(
            transaction_id, from_user_wallet, to_user_wallet, points_used, balance_before, balance_after, status
        )
        VALUES (
            Tcode_id, 
            A, 
            B, 
            d, 
            balance_A,            -- Balance before the transaction
            CASE WHEN txn_status = 'Success' THEN balance_A - d ELSE balance_A END,  -- Balance after transaction or unchanged
            txn_status
        );

        -- Return the appropriate message based on success or failure
        IF txn_status = 'Success' THEN
            RETURN format('Giao dịch thành công');
        ELSE
            RETURN 'Số dư tài khoản của bạn không đủ để thực hiện giao dịch.';
        END IF;
    -- Exception handling block
    EXCEPTION
        WHEN OTHERS THEN
            -- Catch any errors and return a failure message
            RETURN format('Giao dịch bị hủy do lỗi', SQLERRM);
    END;
END;
$$ LANGUAGE plpgsql;


-- show data in table

SELECT * FROM users;
SELECT * FROM Transactioncode;
SELECT * FROM history;

-- delete table
DROP TABLE users;
DROP TABLE Transactioncode;
DROP TABLE history;
DROP FUNCTION complex_transaction(A BIGINT, B BIGINT, d BIGINT);

-- clear data in table
TRUNCATE history;
TRUNCATE users;
TRUNCATE Transactioncode;


-- test case
INSERT INTO users (uname, upassword, balance) VALUES ('Quan', '2312121', 5000);
INSERT INTO users (uname, upassword, balance) VALUES ('Huy', '2312121', 4000);
SELECT complex_transaction(1 ,2, 50000);
SELECT complex_transaction(1 ,2, 2000);
SELECT check_user('Long')
SELECT check_user('Huy')

