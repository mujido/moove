while read class; do
    cat <<EOF
void $class::accept(ASTVisitor& visitor)const
{
   visitor.visit(*this);
}

EOF
done
