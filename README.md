 <div class="info">
    <p><strong>Họ và tên:</strong> Doãn Duy Lợi</p>
    <p><strong>Mã số sinh viên:</strong> 24021549</p>
    <p><strong>Lớp:</strong> K69I-CS1</p>
  </div>

  <h1>🎮 Game: Space Shooter 🚀</h1>

  <div class="section">
    <h2> Giới thiệu</h2>
    <p><strong>Space Shooter</strong> là trò chơi bắn phi thuyền cổ điển phong cách pixel. Bạn sẽ điều khiển một phi thuyền không gian, chiến đấu với làn sóng quái vật ngoài hành tinh đang tấn công Trái Đất.</p>
  </div>

  <div class="section">
    <h2>🕹️ Nội dung và cách chơi game</h2>
    <p>Nội dung: Trò chơi bắt đầu khi phi thuyền của bạn tiến vào vùng không gian. Nhiệm vụ của bạn là tiêu diệt toàn bộ các quái vật xuất hiện và nhận được những phần quà để đạt điểm cao nhất. Bạn sẽ thua khi bị quái vật chạm phải hoặc vượt qua bạn. Mỗi lần bắn trúng quái vật bạn sẽ được 1 point ( mỗi khi bạn thêm được 10 point thì speed của quái vật sẽ tăng lên một con số nhất định), bắn trúng gift bạn sẽ được +1 speed. Hãy sống sót càng lâu càng tốt!</p>
    <p>Cách chơi: Phi thuyền được điều khiển từ các nút trong bàn phím:</p>
    <p>- w: đi lên</p>
    <p>- s: đi xuống</p>
    <p>- a: sang phải</p>
    <p>- d: sang trái</p>
    <p>- shift: dừng</p>
    <p>- space: bắn</p>
    <p>- esc: tạm dừng trò chơi</p>
    <p>- ctrl + các nút di chuyển: tăng tốc</p>
   
  </div>
<div class="section">
  <h2>Hình ảnh có trong game</h2>
  <p>spaceship ( tham khảo từ https://perchance.org/ai-text-to-image-generator và vẽ thêm)</p>
   <img src="spaceship.png">
   
  
   <p>skulls (enemy) (tự vẽ)</p>
   <img src="skulls.png">
   <p>gift (tham khảo trên mạng)</p>
   <img src="gift.png" >
   <p>background (tham khảo trên mạng)</p>
   <img src="cl.jpg">
  <div>
<div class="section">
  <h2>Nguồn sound, file sdl </h2>
  <p>- Sound:</p>
  <p>click.mp3, pop.mp3, reward.mp3, end.mp3: https://tiengdong.com/ </p>
  <p>file sdl: từ github của bài giảng trên lớp</p>
  <p>code: vận dụng kiến thức bản thân và tham khảo code từ AI</p>
<div>
 <div class="section">
   <h2>Code</h2>
  <p>Gồm 1 file main và 19 file headers</p>
  
<div class="section">
  <h2>Thuật toán xử lí va chạm</h2>
  <p>Chia ship thành 4 phần theo chiều ngang, mỗi phần có chiều rộng khác nhau do cấu trúc của phi thuyền. "skull.png" và "gift.png" được tính là va chạm với "spaceship.png" khi tọa độ của các khối đó nằm trong 1 trong 1 phần của phi thuyền. </p>
</body>
</html>
