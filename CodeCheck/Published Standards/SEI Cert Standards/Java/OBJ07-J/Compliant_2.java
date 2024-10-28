class SensitiveClass {
    private char[] filename;
    private Boolean shared = false;

    SensitiveClass(String filename) {
        this.filename = filename.toCharArray();
    }

    final void replace() {
        if (!shared) {
            for (int i = 0; i < filename.length; i++) {
                filename[i] = 'x';
            }
        }
    }

    final String get() {
        if (!shared) {
            shared = true;
            return String.valueOf(filename);
        } else {
            throw new IllegalStateException("Failed to get instance");
        }
    }

    final void printFilename() {
        System.out.println(String.valueOf(filename));
    }

    public final SensitiveClass clone()
            throws CloneNotSupportedException {
        throw new CloneNotSupportedException();
    }
}