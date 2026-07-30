import React, { useState, useRef } from 'react';
import { View, Text, StyleSheet, Pressable } from 'react-native';

// Khuôn nút bấm điều khiển
const ControlButton = ({ 
  label, command, color = '#444', width = 70, height = 70, borderRadius = 15 
}: { 
  label: string, command: string, color?: string, width?: number, height?: number, borderRadius?: number 
}) => {
  // Dùng useRef để lưu trữ bộ đếm thời gian (timer)
  const intervalRef = useRef<any>(null);

  const startSending = (char: string) => {
    console.log("Đang phát tín hiệu:", char);
    
    // Cứ mỗi 100 mili-giây (0.1 giây) nó sẽ tự động gửi lệnh xuống một lần nếu tay vẫn đang đè
    if (intervalRef.current !== null) return;
    intervalRef.current = setInterval(() => {
      console.log("Đang phát tín hiệu (giữ):", char);
    }, 100);
  };

  const stopSending = () => {
    // Khi nhả tay ra -> Xóa bộ đếm và gửi lệnh dừng '0'
    if (intervalRef.current) {
      clearInterval(intervalRef.current);
      intervalRef.current = null;
    }
    console.log("Đang phát tín hiệu: 0 (Dừng lại)");
  };

  return (
    <Pressable
      onPressIn={() => startSending(command)} // Bắt đầu nhấn giữ
      onPressOut={stopSending}                // Nhả tay ra
      style={({ pressed }) => [
        styles.btn,
        { backgroundColor: pressed ? '#777' : color, width, height, borderRadius }
      ]}
    >
      <Text style={styles.btnText}>{label}</Text>
    </Pressable>
  );
};

// Khuôn nút dành riêng cho chức năng Tốc độ (Bật/Tắt)
const SpeedButton = ({ 
  label, speedValue, activeColor, currentSpeed, setSpeed 
}: { 
  label: string, speedValue: number, activeColor: string, currentSpeed: number, setSpeed: (val: number) => void 
}) => {
  const isActive = currentSpeed === speedValue; // Kiểm tra xem nút này có đang được chọn không

  return (
    <Pressable
      onPress={() => {
        // Nếu đang sáng mà bấm lại -> tắt (về 0). Nếu đang tắt -> bật số đó lên
        const newSpeed = isActive ? 0 : speedValue;
        setSpeed(newSpeed);
        console.log("Đang thiết lập tốc độ:", newSpeed);
      }}
      style={({ pressed }) => [
        styles.btn,
        { 
          width: 120, height: 35, borderRadius: 5,
          // Nếu active thì lấy màu sặc sỡ, không thì xám xịt
          backgroundColor: isActive ? activeColor : (pressed ? '#444' : '#222'),
          borderColor: isActive ? activeColor : '#555',
          opacity: isActive ? 1 : 0.5 // Làm mờ chữ và khung nếu đang tắt
        }
      ]}
    >
      <Text style={[styles.btnText, { color: isActive ? '#fff' : '#888' }]}>{label}</Text>
    </Pressable>
  );
};

export default function App() {
  const [isBluetoothConnected, setIsBluetoothConnected] = useState(false);
  const [activeSpeed, setActiveSpeed] = useState(0);
  
  return (
    <View style={styles.container}>
      
      {/* ================= CỘT TRÁI ================= */}
      <View style={styles.sidePanel}>
        {/* Cụm Khuỷu tay & Ngàm kẹp (Nằm góc trên) */}
        <View style={styles.topControlGroup}>
            <View style={styles.row}>
                <ControlButton label="Cùi Lên (T)" command="T" width={60} height={60} />
                <ControlButton label="GẮP (J)" command="J" color="#b30000" width={60} height={60} />
            </View>
            <View style={styles.row}>
                <ControlButton label="Cùi Xuống (G)" command="G" width={60} height={60} />
                <ControlButton label="NHẢ (K)" command="K" color="#004080" width={60} height={60} />
            </View>
        </View>

        {/* D-pad Lái Xe (Nằm góc dưới) */}
        <ControlButton label="Tiến (F)" command="F" />
        <View style={styles.row}>
          <ControlButton label="Trái (L)" command="L" />
          <View style={styles.spacer} />
          <ControlButton label="Phải (R)" command="R" />
        </View>
        <ControlButton label="Lùi (B)" command="B" />
      </View>

      {/* ================= CỘT GIỮA ================= */}
      <View style={styles.centerPanel}>
        
        {/* Nút Tìm/Trạng thái Bluetooth */}
        <View style={styles.topStatus}>
            <Pressable 
              style={({ pressed }) => [
                styles.btnBluetooth, 
                { 
                  // Đổi màu: Nếu đã kết nối thì màu Xanh lá, chưa kết nối thì màu Xanh dương
                  backgroundColor: isBluetoothConnected 
                    ? (pressed ? '#21867a' : '#2a9d8f')  // Màu khi đã kết nối
                    : (pressed ? '#0044cc' : '#0055ff'), // Màu khi chưa kết nối
                  borderColor: isBluetoothConnected ? '#45c4b5' : '#88aaff'
                }
              ]}
              // Khi bấm vào sẽ tự động đảo trạng thái (Từ Chưa -> Rồi, và ngược lại)
              onPress={() => setIsBluetoothConnected(!isBluetoothConnected)}
            >
                <Text style={styles.btnBluetoothText}>
                  {/* Chữ cũng đổi theo trạng thái luôn */}
                  {isBluetoothConnected ? 'ĐÃ KẾT NỐI 🔵' : 'TÌM BLUETOOTH 🔍'}
                </Text>
            </Pressable>
        </View>
        
        {/* Màn hình hiển thị */}
        <View style={styles.screen}>
            {/* Đèn báo an toàn được dời vào đây */}
            <Text style={styles.safeLight}>🟢</Text> 

            {/* Hiển thị Tốc độ (Góc trên phải) */}
            <Text style={styles.speedIndicator}>{activeSpeed}</Text>
            
            <Text style={styles.title}>DISTANCE</Text>
            <Text style={styles.distanceText}>30 cm</Text>
        </View>

        {/* Thanh chỉnh tốc độ */}
        <View style={styles.speedContainer}>
            <Text style={styles.speedTitle}>Speed (Tốc độ)</Text>
            <SpeedButton label="4" speedValue={4} activeColor="#e63946" currentSpeed={activeSpeed} setSpeed={setActiveSpeed} />
            <SpeedButton label="3" speedValue={3} activeColor="#f4a261" currentSpeed={activeSpeed} setSpeed={setActiveSpeed} />
            <SpeedButton label="2" speedValue={2} activeColor="#e9c46a" currentSpeed={activeSpeed} setSpeed={setActiveSpeed} />
            <SpeedButton label="1" speedValue={1} activeColor="#2a9d8f" currentSpeed={activeSpeed} setSpeed={setActiveSpeed} />
        </View>
      </View>

      {/* ================= CỘT PHẢI ================= */}
      <View style={styles.sidePanel}>
        
        {/* Nút Còi Tròn (Nằm góc trên) */}
        <View style={styles.topControlGroup}>
            <ControlButton label="CÒI (V)" command="V" color="#cccc00" width={75} height={75} borderRadius={40} />
        </View>

        {/* D-pad Vai & Đế (Nằm góc dưới) */}
        <ControlButton label="Vai Lên (W)" command="W" />
        <View style={styles.row}>
          <ControlButton label="Đế Trái (A)" command="A" />
          <View style={styles.spacer} />
          <ControlButton label="Đế Phải (D)" command="D" />
        </View>
        <ControlButton label="Vai Xuống (S)" command="S" />
      </View>

    </View>
  );
}

const styles = StyleSheet.create({
  container: { 
    flex: 1, 
    flexDirection: 'row', 
    backgroundColor: '#1e1e1e' 
  },
  sidePanel: { 
    flex: 1, 
    justifyContent: 'flex-end', 
    alignItems: 'center', 
    paddingBottom: 10
  },
  centerPanel: { 
    flex: 0.8, 
    justifyContent: 'space-between', 
    alignItems: 'center', 
    paddingVertical: 20,
    borderLeftWidth: 1,
    borderRightWidth: 1,
    borderColor: '#333' 
  },
  topControlGroup: {
    position: 'absolute', 
    top: 20,
    alignItems: 'center'
  },
  topStatus: { 
    alignItems: 'center',
    marginBottom: 10
  },
  btnBluetooth: {
    paddingHorizontal: 20,
    paddingVertical: 10,
    borderRadius: 25,
    marginBottom: 10,
    borderWidth: 1,
    borderColor: '#88aaff',
    elevation: 3, // Tạo bóng đổ nhẹ
  },
  btnBluetoothText: {
    color: '#ffffff',
    fontWeight: 'bold',
    fontSize: 13,
    letterSpacing: 1
  },
  statusText: { color: '#aaa', fontSize: 13, fontWeight: 'bold' },
  screen: { 
    width: 220,
    height: 110,
    justifyContent: 'center',
    alignItems: 'center', 
    backgroundColor: '#0a0a0a', 
    paddingVertical: 15, 
    paddingHorizontal: 40,
    borderRadius: 8, 
    borderWidth: 2, 
    borderColor: '#444' 
  },
  safeLight: {
    position: 'absolute',
    top: 10,
    left: 15,
    fontSize: 12,
  },
  speedIndicator: {
    position: 'absolute',
    top: 10,
    right: 15,
    color: '#ffcc00', // Tui để màu vàng cho nó nổi bật, công nghệ
    fontSize: 12,
    fontWeight: 'bold',
    letterSpacing: 1
  },
  title: { color: '#888', fontSize: 12, letterSpacing: 2, marginBottom: 5 },
  distanceText: { color: '#00ffcc', fontSize: 45, fontWeight: 'bold' },
  speedContainer: { alignItems: 'center' },
  speedTitle: { color: '#aaa', fontSize: 13, marginBottom: 8 },
  row: { flexDirection: 'row', alignItems: 'center' },
  spacer: { width: 70, height: 70 }, 
  btn: { 
    justifyContent: 'center', 
    alignItems: 'center', 
    margin: 3,
    borderWidth: 1,
    borderColor: '#555'
  },
  btnText: { color: '#fff', fontSize: 12, fontWeight: 'bold', textAlign: 'center' }
});