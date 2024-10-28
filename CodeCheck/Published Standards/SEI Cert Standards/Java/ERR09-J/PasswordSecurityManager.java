class PasswordSecurityManager extends SecurityManager {
    private boolean isExitAllowedFlag;

    public PasswordSecurityManager() {
        super();
        isExitAllowedFlag = false;
    }

    public boolean isExitAllowed() {
        return isExitAllowedFlag;
    }

    @Override
    public void checkExit(int status) {
        if (!isExitAllowed()) {
            throw new SecurityException();
        }
        super.checkExit(status);
    }

    public void setExitAllowed(boolean f) {
        isExitAllowedFlag = f;
    }
}