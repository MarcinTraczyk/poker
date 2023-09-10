from poker import calculate_table_odds

res = calculate_table_odds(
    hero_cards = ['Ah', 'Kh'],
    villains = [['Js', 'Jh']]
)

print("="*32)
print(f"""wins: {100*res[0]:.1f}%
loses: {100*res[1]:.1f}%
draws: {100*res[2]:.1f}%

games simulated: {int(res[3])}
""")