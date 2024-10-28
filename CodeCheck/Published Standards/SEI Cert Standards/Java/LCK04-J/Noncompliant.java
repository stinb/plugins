import java.util.Map;
import java.util.Set;
import java.util.Collections;
import java.util.HashMap;

public class Noncompliant {
    private final Map<Integer, String> mapView = Collections.synchronizedMap(new HashMap<Integer, String>());
    private final Set<Integer> setView = mapView.keySet();

    public Map<Integer, String> getMap() {
        return mapView;
    }

    public void doSomething() {
        synchronized (setView) { // UndCC_Violation, Incorrectly synchronizes on setView
            for (Integer k : setView) {
                // ...
            }
        }
    }

}
