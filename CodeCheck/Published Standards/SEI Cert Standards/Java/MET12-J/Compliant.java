import javax.swing.JFrame;

class MyFrame {
    private JFrame frame;
    private byte[] buffer = new byte[16 * 1024 * 1024]; // Now decoupled
}