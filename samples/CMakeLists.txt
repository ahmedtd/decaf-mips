
# Copy the tester script to the output
configure_file(diff-test.sh ./)

# Make our lives easier with a convenience function
function(comparison_test fragment)
    configure_file("${fragment}" ./ COPYONLY)
    add_test("${fragment}-test" /bin/sh diff-test.sh "${fragment}")
endfunction(comparison_test)

# Copy test fragments and programs to output directory
comparison_test(badbool.frag    )
comparison_test(baddouble.frag  )
comparison_test(badident.frag   )
comparison_test(badint.frag     )
comparison_test(badop.frag      )
comparison_test(badpre.frag     )
comparison_test(badreserve.frag )
comparison_test(badstring.frag  )
comparison_test(comment.frag    )
comparison_test(define.frag     )
comparison_test(ident.frag      )
comparison_test(number.frag     )
comparison_test(program2.decaf  )
comparison_test(program3.decaf  )
comparison_test(reserve_op.frag )
comparison_test(string.frag     )
comparison_test(long_int_constant.frag)
comparison_test(line_noise.frag)