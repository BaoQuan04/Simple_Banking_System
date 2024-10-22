-- create table
CREATE TABLE accounts(
    uname VARCHAR(50) UNIQUE NOT NULL,
    urole VARCHAR(50) NOT NULL
);
CREATE TABLE admins(

);

CREATE TABLE users(
    user_id BIGSERIAL PRIMARY KEY,
    uname VARCHAR(50) UNIQUE NOT NULL,
    upassword VARCHAR(50) DEFAULT '1',
    wallet_id BIGINT REFERENCES wallet(wallet_id)
	--birth_date,
	--CCCD BIGINT UNIQUE,
	--Email VARCHAR(50),
	--address
);

CREATE TABLE totalpoints(
	total BIGINT DEFAULT 50000,
	pointin BIGINT DEFAULT 0,
	pointout BIGINT DEFAULT 0
);

CREATE TABLE Transactioncode(
	wallet_id BIGINT REFERENCES wallet(wallet_id),
	transaction_id BIGSERIAL PRIMARY KEY
);

CREATE TABLE wallet(
	wallet_id BIGSERIAL PRIMARY KEY,
	balance BIGINT NOT NULL
);

CREATE TABLE history(
    transaction_id BIGINT NOT NULL REFERENCES Transactioncode(transaction_id),
	from_user_wallet BIGINT NOT NULL REFERENCES wallet(wallet_id),
    to_user_wallet BIGINT NOT NULL REFERENCES wallet(wallet_id),
    points_used INT NOT NULL,
    balance_before BIGINT NOT NULL,
	balance_after BIGINT NOT NULL,
    date_excute timestamp NOT NULL DEFAULT NOW()::TIMESTAMP(0), -- local time generate when insert into history 
	status VARCHAR(50) NOT NULL
);

-- modify for best performance
CREATE INDEX wallet_id_index ON Transacioncode(wallet_id);
CREATE INDEX transaction_id_index ON history(transaction_id);

-- don't know maybe delete
SELECT urole FROM users WHERE uname = '' /*check uname exist in SQL when logging*/
SELECT * FROM users LIMIT 5 ORDER BY ASC; /*print 5 roll from beginning of users table*/ 


INSERT INTO users(uname, urole) values ('', ''); /*add user or admin to control in system */
INSERT INTO userwithwallet(uname, upassword, upoints) values ();/* add user info */
INSERT INTO history(user_id, from_user, to_user, points_transferred, ubalance) values(); /*add history for tracking*/
UPDATE userwithwallet SET ubalance = ubalance - history.points_transeferred WHERE userwithwallet.user_id = history.user_id; /*update sender's balance*/
UPDATE userwithwallet SET ubalance = ubalance + history.points_transeferred WHERE userwithwallet.uname = history.to_user; /* update receiver's balance */


-- update pointout for each user created
INSERT INTO users(uname, upassword, ) VALUES ();
UPDATE totalpoints SET pointout = pointout + 5000;



-- update user info
CREATE OR REPLACE update_uinfo()
RETURN TEXT AS $$
BEGIN
	UPDATE users SET upassword = new_upassword;
	UPDATE .......

	RETURN 'Cập nhật thành công';
END;
EXCEPTION
    -- Catch any errors and return a failure message
    WHEN OTHERS THEN
        RETURN format('Cập nhật thất bại: %s', SQLERRM);

-- show list of user




-- show transfer log
CREATE OR REPLACE FUNCTION transfer_log(p_wallet_id BIGINT)
RETURNS TABLE(
    date_excute TIMESTAMP, 
    points_used INT, 
    balance_before BIGINT, 
    balance_after BIGINT, 
    status VARCHAR
) AS $$
BEGIN
    -- Start the transaction (implicit in PostgreSQL functions)
    BEGIN
        -- The main query that fetches the transaction log
        RETURN QUERY
        SELECT 
            h.date_excute, 
            h.points_used, 
            h.balance_before, 
            h.balance_after, 
            h.status
        FROM history h
        INNER JOIN Transactioncode t ON h.transaction_id = t.transaction_id
        WHERE t.wallet_id = p_wallet_id
        ORDER BY h.date_excute DESC;

    EXCEPTION
        -- Catch any error that occurs
        WHEN OTHERS THEN
            -- Handle the error and raise a notice or error message
            RAISE NOTICE 'Error occurred, transaction rolled back: %', SQLERRM;
            -- ROLLBACK is implied, so we do not need to explicitly call it
            RETURN;
    END;
END;
$$ LANGUAGE plpgsql;



-- check pointout and pointin when closing system
CREATE OR REPLACE FUNCTION check_total()
RETURNS TEXT AS $$
DECLARE
	total BIGINT;
	pin BIGINT;
	pout BIGINT;
BEGIN
	SELECT pointin INTO pin FROM totalpoints;
	SELECT pointout INTO pout FROM totalpoints;
	IF pin != pout THEN
		BEGIN
			SELECT sum(balance) INTO total FROM wallet;
			pin := pin + total;
			UPDATE wallet SET balance = 0;
			UPDATE totalpoints SET pointin = pin;
		END;
	END IF;
	
	RETURN format('Chuong trinh ket thuc');
	
EXCEPTION
   	WHEN OTHERS THEN
    	-- Catch any errors and return a failure message
        RETURN format('Đã xảy ra lỗi', SQLERRM);
END;
$$ LANGUAGE plpgsql;




-- check username exist in SQL for logging or transfer
CREATE OR REPLACE FUNCTION check_user(name VARCHAR(50))
RETURNS TABLE(wallet_id BIGINT) AS $$
BEGIN
    -- Attempt to find the wallet_id
    RETURN QUERY
    SELECT u.wallet_id
    FROM users u
    WHERE u.uname = name;

    -- If no result is found, it will return NULL by default.

EXCEPTION
    WHEN OTHERS THEN
        -- Catch any errors and return NULL in case of failure
        RETURN QUERY SELECT NULL;
END;
$$ LANGUAGE plpgsql;



-- create user account
CREATE OR REPLACE FUNCTION create_user(uname VARCHAR(50), upassword VARCHAR(50) DEFAULT '1', value NUMERIC DEFAULT 5000)
RETURNS TEXT AS $$
DECLARE
    uwallet_id BIGINT;
BEGIN
    -- Insert a new wallet with the specified balance
    INSERT INTO wallet(balance) VALUES(value)
    RETURNING wallet_id INTO uwallet_id;

    -- Insert a new user with the generated wallet_id
    INSERT INTO users(uname, upassword, wallet_id) VALUES(uname, upassword, uwallet_id);

    -- Return success message
    RETURN 'Tạo tài khoản thành công';

EXCEPTION
    -- Catch any errors and return a failure message
    WHEN OTHERS THEN
        RETURN format('Tạo tài khoản thất bại: %s', SQLERRM);
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
        SELECT balance INTO balance_A FROM wallet WHERE wallet_id = A;
        SELECT balance INTO balance_B FROM wallet WHERE wallet_id = B;

        -- Always generate a transaction_id regardless of success or failure
        INSERT INTO Transactioncode(wallet_id)
        VALUES (A)
        RETURNING transaction_id INTO Tcode_id;

        -- Check if balance of A is sufficient for the transaction
        IF balance_A >= d THEN
            -- Update the balances for wallet A and B
            UPDATE wallet SET balance = balance - d WHERE wallet_id = A;
            UPDATE wallet SET balance = balance + d WHERE wallet_id = B;

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
SELECT * FROM totalpoints;
SELECT * FROM wallet;

-- delete table
DROP TABLE users;
DROP TABLE Transactioncode;
DROP TABLE history;
DROP TABLE totalpoints;
DROP TABLE wallet;
DROP FUNCTION complex_transaction(A BIGINT, B BIGINT, d BIGINT);


-- clear data in table
TRUNCATE history;
TRUNCATE users;
TRUNCATE Transactioncode;


-- test case
SELECT create_user('Quan');
UPDATE totalpoints SET pointout = pointout + 5000;
SELECT create_user('Huy')
UPDATE totalpoints SET pointout = pointout + 5000;
INSERT INTO totalpoints(total) VALUES (50000);
SELECT complex_transaction(1, 2, 50000);
SELECT complex_transaction(1, 2, 2000);
SELECT check_user('Long');
SELECT check_user('Huy');
SELECT check_total();


