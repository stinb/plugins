public final class Diary_n {
  private static final ThreadLocal<Day_n> days = new ThreadLocal<Day_n>() {
    // Initialize to Monday
    protected Day_n initialValue() {
      return Day_n.MONDAY;
    }
  };

  private static Day_n currentDay() {
    return days.get();
  }

  public static void setDay(Day_n newDay) {
    days.set(newDay);
  }

  // Performs some thread-specific task
  public void threadSpecificTask() {
    // Do task ...
  }
}