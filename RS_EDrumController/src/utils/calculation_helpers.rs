pub fn get_limited_value<T: PartialOrd + Copy>(input_value: T, top_limit: T) -> T {
    if input_value > top_limit {
        top_limit
    } else {
        input_value
    }
}
