-- create table
CREATE TABLE accounts(
    acctelephone VARCHAR(50) UNIQUE NOT NULL,
    urole VARCHAR(50) NOT NULL
);

CREATE TABLE admins(
	adname VARCHAR(50) NOT NULL,
	adpassword NUMERIC NOT NULL
);

CREATE TABLE users(
    user_id BIGSERIAL PRIMARY KEY,
    utelephone VARCHAR(50) UNIQUE NOT NULL,
	uname VARCHAR(50) UNIQUE ,
    upassword NUMERIC NOT NUll,
	birth_date VARCHAR(50),
	Email VARCHAR(50),
	sex VARCHAR(50),
	wallet_id BIGINT REFERENCES wallet(wallet_id),
	first_login NUMERIC DEFAULT 0
);

CREATE TABLE totalpoints(
	total BIGINT DEFAULT 50000,
	pointin BIGINT DEFAULT 0,
	pointout BIGINT DEFAULT 0
);

CREATE TABLE Transactioncode(
	from_wallet_id BIGINT REFERENCES wallet(wallet_id),
	to_wallet_id BIGINT REFERENCES wallet(wallet_id),
	transaction_id BIGSERIAL PRIMARY KEY
);

CREATE TABLE wallet(
	wallet_id BIGSERIAL PRIMARY KEY,
	balance BIGINT NOT NULL
);

SELECT nextval('wallet_wallet_id_seq');
SELECT nextval('users_user_id_seq');
SELECT currval('wallet_wallet_id_seq'::regclass);
SELECT currval('users_user_id_seq'::regclass);



CREATE TABLE history(
    transaction_id BIGINT NOT NULL REFERENCES Transactioncode(transaction_id),
	from_user_wallet BIGINT NOT NULL REFERENCES wallet(wallet_id),
    to_user_wallet BIGINT NOT NULL REFERENCES wallet(wallet_id),
    points_used INT NOT NULL,
    A_balance_before BIGINT NOT NULL,
	A_balance_after BIGINT NOT NULL,
	B_balance_before BIGINT NOT NULL,
	B_balance_after BIGINT NOT NULL,
    date_excute timestamp NOT NULL DEFAULT NOW()::TIMESTAMP(0), -- local time generate when insert into history 
	status VARCHAR(50) NOT NULL
);


-- modify for best performance
CREATE INDEX from_wallet_id_index ON Transactioncode(from_wallet_id);
CREATE INDEX to_wallet_id_index ON Transactioncode(to_wallet_id);
CREATE INDEX TR_transaction_id_index ON Transactioncode(transaction_id);
CREATE INDEX utelephone_index ON users(utelephone);
CREATE INDEX accname_index ON accounts(acctelephone);
CREATE INDEX uwallet_id_index ON users(wallet_id);
CREATE INDEX HT_transaction_id_index ON history(transaction_id);



-- update pointout for each user created
INSERT INTO users(uname, upassword, ) VALUES ();
UPDATE totalpoints SET pointout = pointout + 5000;


--check log in
CREATE OR REPLACE FUNCTION login(telephone_input VARCHAR(50))
RETURNS TABLE(
    utelephone VARCHAR(50),
	uname VARCHAR(50),
    upassword NUMERIC, 
    wallet_id BIGINT, 
    urole VARCHAR(50)
) LANGUAGE plpgsql AS $$
DECLARE
    user_role VARCHAR(50);  -- Biến lưu vai trò của người dùng
BEGIN
    -- Kiểm tra xem có bản ghi nào trong bảng accounts khớp với name_input không
    IF NOT EXISTS (
        SELECT 1
        FROM accounts AS a
        WHERE a.acctelephone = telephone_input
    ) THEN
        -- Nếu không có bản ghi nào khớp, trả về "0" cho tất cả các cột
        RETURN QUERY SELECT '0'::VARCHAR(50), '0'::VARCHAR(50), 0::NUMERIC, 0::BIGINT, '0'::VARCHAR(50);
    ELSE
        -- Nếu tìm thấy bản ghi, lấy vai trò của người dùng từ bảng accounts
        SELECT a.urole INTO user_role FROM accounts AS a WHERE a.acctelephone = telephone_input;

        -- Dựa vào vai trò người dùng, trả về dữ liệu từ bảng admin hoặc users
        IF user_role = 'admin' THEN
            RETURN QUERY 
            SELECT ad.adname, '0'::VARCHAR(50), ad.adpassword, 0::BIGINT, user_role
            FROM admins AS ad
            WHERE ad.adname = telephone_input;
        ELSE
            RETURN QUERY 
            SELECT u.utelephone, u.uname, u.upassword, u.wallet_id, user_role
            FROM users AS u
            WHERE u.utelephone = telephone_input;
        END IF;
    END IF;
END;
$$;


--update user info
CREATE OR REPLACE FUNCTION update_uinfo(
    input_wallet_id BIGINT, 
    new_uname VARCHAR(50) DEFAULT NULL, 
    new_upassword NUMERIC DEFAULT NULL,
    new_birth VARCHAR(50) DEFAULT NULL,
    new_email VARCHAR(50) DEFAULT NULL,
    new_sex VARCHAR(50) DEFAULT NULL
)
RETURNS TEXT AS $$
BEGIN
    -- Cập nhật trực tiếp với các giá trị không phải NULL
    UPDATE users
    SET 
        uname = COALESCE(new_uname, uname),  -- Nếu new_uname không NULL, cập nhật; ngược lại giữ nguyên
        upassword = COALESCE(new_upassword, upassword),
        birth_date = COALESCE(new_birth, birth_date),
        email = COALESCE(new_email, email),
        sex = COALESCE(new_sex, sex)
    WHERE wallet_id = input_wallet_id;

    RETURN 'Cap nhat thanh cong';
EXCEPTION
    WHEN OTHERS THEN
        RETURN format('Cap nhat that bai: %s', SQLERRM);
END;
$$ LANGUAGE plpgsql;






-- show list of user
-- change password



-- show transfer log
CREATE OR REPLACE FUNCTION transfer_log(p_wallet_id BIGINT, track_number BIGINT)
RETURNS TABLE(
    "Ngay thuc hien" TIMESTAMP,
    "Noi dung" TEXT,
    "Diem su dung" INT, 
    "So du truoc giao dich" BIGINT, 
    "So du sau giao dich" BIGINT,
    "Trang thai giao dich" TEXT
) AS $$
BEGIN
    RETURN QUERY
    SELECT 
        h.date_excute AS "Ngay thuc hien",
        
        -- Xác định nội dung dựa trên cột `from_user_wallet` hoặc `to_user_wallet`
        CASE 
            WHEN h.from_user_wallet = p_wallet_id THEN 'Chuyen diem den ' || COALESCE(u_to.utelephone, 'Không rõ')
            ELSE 'Nhan diem tu ' || COALESCE(u_from.utelephone, 'Khong ro')
        END AS "Noi dung",
        
        h.points_used AS "Diem su dung",

        -- Xác định "So du truoc giao dich" dựa trên nội dung
        CASE 
            WHEN h.from_user_wallet = p_wallet_id THEN h.A_balance_before
            ELSE h.B_balance_before
        END AS "So du truoc giao dich",
        
        -- Xác định "So du sau giao dich" dựa trên nội dung
        CASE 
            WHEN h.from_user_wallet = p_wallet_id THEN h.A_balance_after
            ELSE h.B_balance_after
        END AS "So du sau giao dich",

        h.status::TEXT AS "Trang thai giao dich"
    FROM history h
    INNER JOIN Transactioncode t ON h.transaction_id = t.transaction_id
    LEFT JOIN users u_from ON h.from_user_wallet = u_from.wallet_id
    LEFT JOIN users u_to ON h.to_user_wallet = u_to.wallet_id
    WHERE 
        (h.from_user_wallet = p_wallet_id)  -- Luôn hiển thị nếu người dùng là người gửi
        OR (h.to_user_wallet = p_wallet_id AND h.status = 'Success')  -- Hiển thị khi là người nhận và trạng thái giao dịch 'thành công'
    ORDER BY h.date_excute DESC
    LIMIT 5
    OFFSET track_number;
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
        RETURN format('Da xay ra loi: ', SQLERRM);
END;
$$ LANGUAGE plpgsql;




-- check username exist in SQL for logging or transfer
CREATE OR REPLACE FUNCTION check_user(telephone_input VARCHAR(50))
RETURNS TABLE(utelephone VARCHAR(50), wallet_id BIGINT) AS $$
BEGIN
    -- Thử tìm tên và wallet_id của người dùng
    RETURN QUERY
    SELECT u.utelephone, u.wallet_id
    FROM users u
    WHERE u.utelephone = telephone_input;

    -- Nếu không tìm thấy, trả về mặc định '0' và 0
    IF NOT FOUND THEN
        RETURN QUERY SELECT '0'::VARCHAR, 0::BIGINT;
    END IF;
END;
$$ LANGUAGE plpgsql;


--check wallet_id valid
CREATE OR REPLACE FUNCTION check_wallet_id(input_wallet BIGINT)
RETURNS TABLE(wallet_id BIGINT) AS $$
BEGIN
	RETURN QUERY
	SELECT u.wallet_id 
	FROM users u
	WHERE u.wallet_id = input_wallet;
	
	IF NOT FOUND THEN
		RETURN QUERY SELECT 0::BIGINT;
	END IF;
END;
$$ LANGUAGE plpgsql;


-- create user account
CREATE OR REPLACE FUNCTION create_user(input_telephone VARCHAR(50), upassword NUMERIC, value NUMERIC DEFAULT 5000)
RETURNS TEXT AS $$
DECLARE
    uwallet_id BIGINT;
BEGIN
    -- Insert a new wallet with the specified balance
    INSERT INTO wallet(balance) VALUES(value)
    RETURNING wallet_id INTO uwallet_id;
	
	INSERT INTO accounts(acctelephone, urole) VALUES (input_telephone, 'user');

    -- Insert a new user with the generated wallet_id and update pointout
    INSERT INTO users(utelephone, upassword, wallet_id) VALUES(input_telephone, upassword, uwallet_id);
	UPDATE totalpoints SET pointout = pointout + 5000;
	
	-- identify first login
	UPDATE users SET first_login = 1 WHERE users.utelephone = input_telephone;
    -- Return success message
    RETURN 'Tao tai khoan thanh cong';

EXCEPTION
    -- Catch any errors and return a failure message
    WHEN OTHERS THEN
        RETURN format('Da xay ra loi vui long thu lai: %s', SQLERRM);
END;
$$ LANGUAGE plpgsql;


--Kiểm tra số tiền trước khi chuyển điểm
CREATE OR REPLACE FUNCTION check_user_balance(input_wallet_id BIGINT)
RETURNS TABLE(ubalance BIGINT) AS $$
BEGIN
	RETURN QUERY
	SELECT balance FROM wallet WHERE wallet.wallet_id = input_wallet_id;
END;
$$ LANGUAGE plpgsql;


-- transfer and catch exceptions
CREATE OR REPLACE FUNCTION complex_transaction(A BIGINT, B BIGINT, d BIGINT)
RETURNS TEXT AS $$
DECLARE
    balance_A BIGINT;
    balance_B BIGINT;
    Tcode_id BIGINT;
BEGIN
    
		-- Kiểm tra số dư trước khi giao dịch
        SELECT balance INTO balance_A FROM wallet WHERE wallet_id = A;
     
		
		-- Lấy số dư ví của B
       	SELECT balance INTO balance_B FROM wallet WHERE wallet_id = B;
		
        -- Tạo transaction_id 
		INSERT INTO Transactioncode(from_wallet_id, to_wallet_id)
        VALUES (A, B)
        RETURNING transaction_id INTO Tcode_id;
			
		-- bắt đầu giao dịch
		BEGIN
			
            -- Cập nhật số dư cho ví A và B
            UPDATE wallet SET balance = balance - d WHERE wallet_id = A;
            UPDATE wallet SET balance = balance + d WHERE wallet_id = B;
			
			
			-- Ghi vào history
			INSERT INTO history(
            transaction_id, from_user_wallet, to_user_wallet, points_used, A_balance_before, A_balance_after, B_balance_before, B_balance_after, status
        	)
        	VALUES (
            	Tcode_id, 
            	A, 
            	B, 
            	d, 
            	balance_A,  -- Số dư của A trước khi giao dịch
            	balance_A - d,  -- Số dư của A sau khi giao dịch thành công
            	balance_B,  -- Số dư ban đầu của B
            	balance_B + d, -- Số dư của B sau khi giao dịch thành công
            	'Success'
        	);
			RETURN format('Giao dich thanh cong.');
           

    -- Khối xử lý ngoại lệ
    EXCEPTION
        WHEN OTHERS THEN
			-- Nếu gặp lỗi vẫn sẽ khi vào trong history và transaction
			BEGIN
				INSERT INTO Transactioncode(from_wallet_id, to_wallet_id)
        		VALUES (A, B)
        		RETURNING transaction_id INTO Tcode_id;
				INSERT INTO history(
            	transaction_id, from_user_wallet, to_user_wallet, points_used, A_balance_before, A_balance_after, B_balance_before, B_balance_after, status
        		)
        		VALUES (
            		Tcode_id, 
            		A, 
            		B, 
            		d, 
            		balance_A,  -- Số dư ban đầu của A
            		balance_A,  -- Số dư của A sau khi giao dịch thất bại
            		balance_B,  -- Số dư ban đầu của B
            		balance_B, 	-- Số dư của B sau khi giao dịch thất bại
            		'Failed'
        		);
				EXCEPTION
                WHEN OTHERS THEN
                    -- Nếu gặp lỗi thì dừng lại, không in ra lỗi
                    NULL;
            END;
            RETURN format('Giao dich bi huy !');
		END;
END;
$$ LANGUAGE plpgsql;





-- show data in table
SELECT * FROM accounts;
SELECT * FROM admins;
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
DROP TABLE accounts;
DROP TABLE admins;
DROP FUNCTION complex_transaction(A BIGINT, B BIGINT, d BIGINT);
DROP FUNCTION check_user(name VARCHAR(50));
DROP FUNCTION transfer_log(p_wallet_id BIGINT, track_number BIGINT);
DROP FUNCTION login(name_input VARCHAR(50));
DROP FUNCTION update_uinfo(bigint,numeric);


-- clear data in table
TRUNCATE history;
TRUNCATE users;
TRUNCATE Transactioncode;
TRUNCATE accounts;
TRUNCATE admins;


-- test case
INSERT INTO admins(adname, adpassword) VALUES ('Quan', '2185');
INSERT INTO accounts(acctelephone, urole) VALUES ('Quan', 'admin');
INSERT INTO accounts(uname, urole) VALUES ('Quan', 'user');
INSERT INTO accounts(uname, urole) VALUES ('Huy', 'user');
SELECT create_user('0905191495', 49);
SELECT create_user('Duy', 1);
UPDATE totalpoints SET pointout = pointout + 5000;
SELECT create_user('Huy')
UPDATE totalpoints SET pointout = pointout + 5000;
INSERT INTO totalpoints(total) VALUES (50000);
SELECT complex_transaction(1, 2, 50000);
SELECT complex_transaction(1, 2, 2000);
SELECT complex_transaction(2, 1, 1000);
SELECT * FROM check_user('0905191495');
SELECT * FROM check_user('Huy');
SELECT * FROM transfer_log(2, 0);
SELECT * FROM transfer_log(1, 0);
SELECT * FROM login('0905191495');
SELECT * FROM login('Quan');
SELECT check_total();
SELECT * FROM check_user('Dang');
SELECT * FROM check_wallet_id(2);
SELECT * FROM update_uinfo(1, NULL, NULL, NULL, 'Nu');
SELECT INDEX FROM
						  

