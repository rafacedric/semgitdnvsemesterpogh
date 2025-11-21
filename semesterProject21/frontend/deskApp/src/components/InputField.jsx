export default function InputField(props) {
    const base_class = "w-full text-center focus:shadow-lg placeholder-gray-700 p-4 mb-2 border rounded-full border-black focus:outline-none"
    return (<input className={ base_class }{...props} />)
}
