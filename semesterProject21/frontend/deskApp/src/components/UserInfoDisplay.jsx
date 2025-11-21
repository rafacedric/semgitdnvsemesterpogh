export default function UserInfoDisplay({ children }) {
    const base = "rounded-full border border-[#33A95E] h-8 w-60 m-4 text-[#33A95E] font-extralight hover:shadow-sm transition-all"
    return (
        <>
            <div className={base}>{children}</div>
        </>
    )
}