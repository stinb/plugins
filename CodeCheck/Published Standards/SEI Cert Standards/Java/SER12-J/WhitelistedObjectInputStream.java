import java.io.*;
import java.util.*;

class WhitelistedObjectInputStream extends ObjectInputStream {
    public Set whitelist;

    public WhitelistedObjectInputStream(InputStream inputStream, Set wl) throws IOException {
        super(inputStream);
        whitelist = wl;
    }

    protected Class<?> resolveClass(ObjectStreamClass cls) throws IOException, ClassNotFoundException {
        if (!whitelist.contains(cls.getName())) {
            throw new InvalidClassException("Unexpected serialized class", cls.getName());
        }
        return super.resolveClass(cls);
    }
}