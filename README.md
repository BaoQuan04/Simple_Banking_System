# Hệ thống đăng nhập và đăng ký tài khoản và quản lý ví điểm thưởng
 
Dự án Hệ thống đăng nhập và đăng ký chủ yếu liên quan đến quá trình đăng ký người dùng. Thông tin xác thực người dùng như tên người dùng và mật khẩu được yêu cầu từ người dùng. Nếu người dùng đăng ký thành công thì với thông tin xác thực được cung cấp, một      bản ghi (có thể là tập tin) của người dùng cụ thể sẽ được tạo trong cơ sở dữ liệu (CSDL).
Ví điểm thưởng là cơ chế quản lý các lượng (đơn vị : điểm) được dùng để quy đổi ra hàng hóa (còn gọi là hoạt động mua - bán). Giữa các ví có giao dịch chuyển điểm.

## I. YÊU CẦU
### A. Các chức năng:

1 - Tạo mới tài khoản khi đăng ký: tạo mới bộ dữ liệu liên quan đến tài khoản người dùng

Người dùng nhập dữ liệu để tạo tài khoản. 
Có thể có nhân viên nhập liệu (người quản lý) để tạo tài khoản hộ người dùng, với điều kiện người dùng cung cấp đầy đủ thông tin cá nhân theo yêu cầu tạo tài khoản.
Hãy phân tích và đề xuất một cấu trúc dữ liệu (lớp) quản lý thông tin tài khoản người dùng.


2 - Lưu trữ dữ liệu: lưu lại dữ liệu tài khoản người dùng xuống tập tin.

Hãy đề xuất việc lưu trữ dữ liệu người dùng dưới dạng tập tin riêng cho từng người dùng hay một tập tin cho tất cả người dùng? Nêu lý do lựa chọn giải pháp.
Với mật khẩu, hãy tìm hiểu cách lưu mật khẩu của người dùng vào CSDL. 
  > [!NOTE]
  > Gợi ý: dùng hàm băm (hash function).

Mọi lưu trữ phải có bản sao lưu (backup). Hãy tìm hiểu quy trình sao lưu - phục hồi dữ liệu. Đề xuất giải pháp sao lưu cho dữ liệu người dùng


3 - Quản lý đăng nhập

 3.1 - Sinh mật khẩu tự động: 
 Sinh một mật khẩu tự động kèm theo thông tin đăng nhập của người dùng trong trường hợp người dùng không cung cấp mật khẩu. 
 Trường hợp này thường được sử dụng khi nhân viên quản lý mở tài khoản mới và người dùng không thể trực tiếp nhập mật khẩu cho mình.

 

 3.2 - Thay đổi mật khẩu: một người dùng có thể tự thay đổi mật khẩu đăng nhập của mình.
Hãy tìm hiểu quy trình xử lý thay đổi mật khẩu.
Với trường hợp mật khẩu tự sinh, hệ thống phải kiểm tra mật khẩu là tự sinh và yêu cầu người dùng thay đổi ngay mật khẩu mới trong lần đăng nhập đầu tiên của mình.


 3.3 - OTP - One Time Password: bảo mật hai lớp để xác thực giao dịch.
    OTP được sử dụng trong đồ án này để xác thực các hoạt động thay đổi thông tin quan trọng của người dùng.
    Hãy tìm hiểu về OTP và đề xuất giải pháp sử dụng OTP cho đồ án (phần B). Xem nguồn tham khảo [1] và [2] và tìm thêm các tài liệu khác về OTP.


 3.4 - Đăng nhập:  
    
Khi người dùng đăng nhập bằng tài khoản của mình, chương trình phải tìm kiếm trong dữ liệu đã lưu trữ và xác định liệu thông tin đăng nhập đó có tồn tại và hợp lệ không. Nếu hợp lệ, cho phép người dùng vào hệ thống và sử dụng các chức năng tương ứng

### B. Phân chia người dùng và chức năng

Nhóm người dùng gồm: 

a/ Người dùng thông thường chỉ được phép truy xuất thông tin cá nhân của mình. 

Được phép điều chỉnh tên, mật khẩu v.v. Hãy căn cứ vào quản lý thông tin tài khoản trên để đưa ra yêu cầu cho phép điều chỉnh, thay đổi trường dữ liệu cụ thể và viết các chức năng cập nhật tương ứng.

 

Khi điều chỉnh xong, hệ thống gửi một mã OTP đến chủ tài khoản cùng thông báo các nội dung sẽ thay đổi để chủ tài khoản xác nhận thao tác cập nhật.

Nếu mã OTP do chủ tài khoản nhập vào hợp lệ, hệ thống tiến hành cập nhật thông tin của người dùng.

 

b/ Người dùng quản lý: Ngoài chức năng quản lý thông tin cá nhân, người quản lý có thể:

 - Theo dõi danh sách nhóm

 - Tạo thêm tài khoản mới

 - Điều chỉnh thông tin của tài khoản khi có yêu cầu từ chủ tài khoản (làm hộ). Khi điều chỉnh xong, hệ thống gửi một mã OTP đến chủ tài khoản cùng thông báo các nội dung sẽ thay đổi để chủ tài khoản xác nhận thao tác cập nhật.

  Nếu mã OTP do chủ tài khoản nhập vào hợp lệ, hệ thống tiến hành cập nhật thông tin của người dùng.

> [!NOTE]
> Chú ý: Không được phép thay đổi tên tài khoản đăng nhập.

 

### C. Quản lý hoạt động ví:

Mỗi người dùng có một bộ dữ liệu về số điểm (ví - wallet) và dữ liệu các giao dịch trao đổi, chuyển điểm từ một ví sang một ví khác.

Một ví có mã số định danh duy nhất phân biệt với tất cả các ví còn lại.

Ví tổng: nguồn duy nhất sinh ra tổng số điểm sẽ lưu chuyển trong toàn bộ hệ thống.
 

Giao dịch chuyển d điểm từ ví A sang ví B sẽ gồm các thao tác thành phần sau

1_ Tìm, mở ví A. Đây là ví chủ của giao dịch, là ví của chủ tài khoản muốn thực hiện chuyển điểm đi.

2_ Tìm, mở ví B. Đây là ví đích của giao dịch. Nếu mã ví B tồn tại, tiếp tục đến 3_Giao dịch

3_ Giao dịch: Hai tác vụ dưới đây là không tách rời (atomic). Bất kỳ có một bất thường (exception) nào xảy ra trong một tác vụ đều dẫn đến hủy toàn bộ giao dịch, phục hồi lại trạng thái số dư của hai ví A và B trước 3_

- Bước 1:
       
       if (A.balance >=d) 
       
                A.balance = A.balance - d

       else

               "low balance. Cannot proceed.”. Đến 4_

- Bước 2:
  
      B.balance = B.balance + d

4_ Kết thúc

 

- Hãy đề xuất cách sử dụng OTP để xác nhận thực hiện giao dịch chuyển điểm giữa các ví.
 

Hệ thống ghi nhận lại giao dịch này trong lịch sử giao dịch (transaction log)

Báo cáo: hệ thống cho phép người dùng theo dõi số dư, lịch sử giao dịch (và trạng thái thực hiện giao dịch)

> Nguồn tham khảo
> 
> [1] CPP_OTP, https://github.com/patzol768/cpp-otp, truy cập lần cuối ngày 04/10/2024.
> 
> [2] COTP, https://github.com/tilkinsc/COTP, truy cập lần cuối ngày 04/10/2024.
> 
> [3] ACID, https://200lab.io/blog/acid-la-gi/, truy cập lần cuối ngày 04/10/2024.

## II. MÔ TẢ HỆ THỐNG
### A. Tổng quan
Đây là một chương trình _**Hệ thống đăng nhập, đăng ký tài khoản và quản lý ví điểm thưởng**_ đơn giản được làm bởi ngôn ngữ C++ kết hợp với SQL, cho phép người dùng có thể đăng nhập, chỉnh sửa, bổ sung thông tin, giao dịch chuyển điểm dựa trên tài khoản được bên admin cung cấp. 
Khi tạo tài khoản ví tổng (chứa 50.000 points ) sẽ cung cấp cho người dùng 5.000 points ban đầu. Các quá trình giao dịch sẽ được lưu lại trên database để theo dõi. 
Người dùng sẽ có các chức năng cơ bản để thực hiện giao dịch và quản lí các thông tin liên quan. Admin sẽ quản lí người dùng thông qua việc tạo tài khoản và xem lịch sử tất cả các giao dịch được lưu trên hệ thống.  

### B. Phân chia người dùng và các chức năng
**Người Dùng Thông Thường (User ):** Phần người dùng thông thường sẽ bao gồm các chức năng quản lí, chỉnh sửa, bổ sung thông tin cá nhân, giao dịch điểm trên hệ thống ví tổng.
- Tạo tài khoản người dùng : người dùng sử dụng số điện thoại để tạo tài khoản, đảm bảo số điện thoại chính xác và đúng định dạng vì số điện thoại sẽ là nơi nhận mã OTP.
- Sau khi tài khoản đã được tạo trên hệ thống thì người dùng sẽ được yêu cầu cập nhật các thông tin vào lần đăng nhập đầu tiên, bao gồm:
  - Họ và tên : Đã được chuẩn hóa viết hoa đầu chữ và viết thường các chữ sau đó( Ví dụ : nGuYen sAng -> Nguyen Sang ).
  - Giới tính : Phải nhập đúng định dạng “nam” hoặc “nu”.Đã chuẩn hóa viết hoa viết thường (Ví dụ : naM -> Nam ).
  - Ngày tháng năm sinh: Phải nhập đúng theo dạng DD/MM/YYYY.Ngày tháng phải là các chữ số và đã được kiểm tra đúng ngày theo tháng và theo năm nhuận (trường hợp của tháng 2). Đã chuẩn hóa D/M/YYYY về dạng DD/MM/YYYY (Ví dụ : 2/2/2005 -> 02/02/2005 ).
  - Email : Địa chỉ email phải nhập đúng định dạng tất cả phải là chữ thường, có đuôi “@gmail.com”.Đã xử lý ngoại lệ trường hợp nhập mỗi “@gmail.com”.
  - Sau khi nhập thông tin xong thì sẽ có mã OTP gồm có 6 chữ số gửi đến tài khoản để xác nhận. Các thông tin không đúng định dạng sẽ trả về NULL. Mã ví sẽ được tạo tự động bởi hệ thống.
- Menu chức năng của người dùng :
  - Xem thông tin : hiển thị tên người dùng, số điện thoại, email, giới tính, mã ví.
  - Thay đổi thông tin : Thay đổi được một hoặc tất cả thông tin liên quan phải đúng định dạng ban đầu nhưng không thể thay đổi được số điện thoại. Sau khi thực hiện xong thao tác sẽ có mã OTP gửi đến để xác nhận.
  - Quản lí điểm thưởng : bao gồm các chức năng :
    -  	Chuyển điểm : Số điểm giao dịch phải là một số dương nhỏ hơn hoặc bằng số điểm đang có trong tài khoản, điểm phải được chuyển sang một ví đã tồn tại, nếu có ngoại lệ trong quá trình chuyển điểm thì hệ thống sẽ dừng quá trình lại nhưng vẫn lưu trong lịch sử hệ thống với trạng thái “Failed ” nếu không thì hệ thống sẽ gửi mã OTP để xác nhận giao dịch.
    -  	Kiểm tra số dư : Cho phép xem số điểm hiện tại trong ví của người dùng.
    -  	Xem lịch sử giao dịch : Cho phép xem 5 giao dịch gần nhất, có thể xem tiếp các giao dịch trước đó nữa thông qua chức năng “xem them”.
**Người Dùng Quản Lí (Admin):** Admin chỉ được phép tạo tài khoản, xem thông tin người dùng và lịch sử giao dịch hệ thống. Menu chức năng của admin:
- Tạo tài khoản : Username bằng số điện thoại (bắt đầu bằng chữ số 0, đủ 10 chữ số và không có chữ cái hay kí tự đặc biệt), mật khẩu là “1” là mật khẩu tự sinh khi admin tạo tài khoản cho người dùng.  Vào lần đầu tiên đăng nhập người dùng phải thay đổi mật khẩu.
- Thay đổi thông tin khách hàng : Có thể thay đổi thông tin bất kì khách hàng nào tương tự như user.
- Quản lý khách hàng : quản lí danh sách tài khoản user.

[Hình ảnh tổng quát về hệ thống](https://app.diagrams.net/#G1ykXi7vlEHZwDp1cat16tMIvEFkWB4I2q#%7B"pageId"%3A"1NUcVHQgvSqW2X4E0K4g"%7D)

> Thành viên nhóm:
> 
> Huỳnh Quốc Huy- N23DCCN161: Tổng hợp các hàm, tạo giao diện menu.
> 
> Nguyễn Trần Bảo Quân- N23DCCN185: Thiết kế và kết nối cơ sở dữ liệu.
> 
> Sang Xuân Hùng- N23DCCN159 : Làm các hàm đăng nhập, các chức năng của ví.
> 
> Nguyễn Võ Phi Long- N23DCCN171: Làm các hàm chuẩn hóa, OTP, hàm băm.
> 
> Trần Thị Cam- Mô tả chương trình, hỗ trợ xây dựng menu.
